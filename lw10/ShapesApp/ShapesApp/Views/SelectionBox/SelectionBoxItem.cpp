#include "SelectionBoxItem.h"
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include "../Factory/QtGraphicsItemFactory.h"

SelectionBoxItem::SelectionBoxItem(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{
    setFlags(ItemIgnoresParentOpacity);
    setZValue(0);

    QPen pen(kBorderColor, kBorderWidth, kBorderStyle);
    pen.setCosmetic(true); 
    setPen(pen);
    setBrush(Qt::NoBrush);
    CreateHandles();
}

void SelectionBoxItem::SetTargetItem(QGraphicsItem* item)
{
    m_targetItem = item;
    UpdateGeometry();
}

void SelectionBoxItem::UpdateGeometry()
{
    if (!m_targetItem) 
        return;

    QRectF sceneRect = m_targetItem->sceneBoundingRect();

    QRectF rect = mapFromScene(sceneRect).boundingRect();
    setRect(rect);
    PositionHandles();
}

void SelectionBoxItem::HandleResizeMove(const QRectF& newBoundingBox)
{
    setRect(newBoundingBox);
    PositionHandles();

    if (m_targetItem) 
    {
        QRectF itemRect = m_targetItem->mapRectFromScene(newBoundingBox);

        if (m_targetItem) 
        {
            if (auto ellipse = dynamic_cast<QGraphicsEllipseItem*>(m_targetItem))
            {
                ResizeEllipseItem(ellipse, itemRect);
            }
            else if (auto rect = dynamic_cast<QGraphicsRectItem*>(m_targetItem))
            {
                ResizeRectItem(rect, itemRect);
            }
            else if (auto polygon = dynamic_cast<QGraphicsPolygonItem*>(m_targetItem))
            {
                ResizeTriangleItem(polygon, itemRect);
            }
            else if (auto image = dynamic_cast<QGraphicsPixmapItem*>(m_targetItem))
            {
                ResizeImageItem(image, newBoundingBox);
            }
        }
    }
}

void SelectionBoxItem::HandleHandleMove(HandleType type, const QPointF& delta)
{
    emit resizeRequested(type, delta.x(), delta.y());
}

void SelectionBoxItem::HandleResizeRelease()
{
    emit resizeFinished();
}

void SelectionBoxItem::CreateHandles()
{
    m_handles[0] = new ResizeHandle(HandleType::TopLeft, this);
    m_handles[1] = new ResizeHandle(HandleType::TopRight, this);
    m_handles[2] = new ResizeHandle(HandleType::BottomLeft, this);
    m_handles[3] = new ResizeHandle(HandleType::BottomRight, this);

    connect(m_handles[0], &ResizeHandle::dragMove, this, &SelectionBoxItem::HandleHandleMove);
    connect(m_handles[1], &ResizeHandle::dragMove, this, &SelectionBoxItem::HandleHandleMove);
    connect(m_handles[2], &ResizeHandle::dragMove, this, &SelectionBoxItem::HandleHandleMove);
    connect(m_handles[3], &ResizeHandle::dragMove, this, &SelectionBoxItem::HandleHandleMove);

    PositionHandles();
}

void SelectionBoxItem::PositionHandles()
{
    if (!m_targetItem) 
        return;

    QRectF rect = this->rect(); 
    qreal hs = kHandleSize / 2;

    m_handles[0]->setPos(rect.topLeft());      
    m_handles[1]->setPos(rect.topRight());     
    m_handles[2]->setPos(rect.bottomLeft());   
    m_handles[3]->setPos(rect.bottomRight()); 
}

void SelectionBoxItem::ResizeImageItem(QGraphicsPixmapItem* image, const QRectF& newBox)
{
    if (!image) return;

    QPixmap original = image->data(QtGraphicsItemFactory::OriginalPixmapRole).value<QPixmap>();
    if (original.isNull()) return;

    int targetWidth = qAbs(static_cast<int>(newBox.width()));
    int targetHeight = qAbs(static_cast<int>(newBox.height()));

    // Растягиваем без сохранения пропорций
    QPixmap scaled = original.scaled(targetWidth, targetHeight,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);

    // Позиционируем
    double posX = newBox.width() < 0 ? newBox.right() : newBox.left();
    double posY = newBox.height() < 0 ? newBox.bottom() : newBox.top();

    image->setPixmap(scaled);
    image->setPos(posX, posY);

    image->setData(QtGraphicsItemFactory::TargetSizeRole, QSizeF(targetWidth, targetHeight));
}

void SelectionBoxItem::ResizeRectItem(QGraphicsRectItem* rect, const QRectF& newBox)
{
    rect->setRect(newBox);
}

void SelectionBoxItem::ResizeTriangleItem(QGraphicsPolygonItem* triangle, const QRectF& newBox)
{
    if (!triangle) return;

    QPolygonF polygon = triangle->polygon();
    if (polygon.isEmpty()) return;

    QRectF currentBox = polygon.boundingRect();
    double currentWidth = currentBox.width();
    double currentHeight = currentBox.height();

    if (qFuzzyIsNull(currentWidth)) currentWidth = 1.0;
    if (qFuzzyIsNull(currentHeight)) currentHeight = 1.0;
 
    bool flipX = (newBox.width() < 0);
    bool flipY = (newBox.height() < 0);

    double newLeft = flipX ?
        newBox.right() :
        newBox.left();

    double newTop = flipY ?
        newBox.bottom() :
        newBox.top();

    double newWidth = qAbs(newBox.width());
    double newHeight = qAbs(newBox.height());

    QPolygonF newPolygon;
    for (const QPointF& point : polygon)
    {
        double relX = (point.x() - currentBox.left()) / currentWidth;
        double relY = (point.y() - currentBox.top()) / currentHeight;

        if (flipX) relX = 1.0 - relX;
        if (flipY) relY = 1.0 - relY;

        // Вычисляем новые абсолютные координаты
        double newX = newLeft + relX * newWidth;
        double newY = newTop + relY * newHeight;

        newPolygon.append(QPointF(newX, newY));
    }

    triangle->setPolygon(newPolygon);
    triangle->setPos(0, 0);
    triangle->resetTransform();
}

void SelectionBoxItem::ResizeEllipseItem(QGraphicsEllipseItem* ellipse, const QRectF& newBox)
{
    ellipse->setRect(newBox);
}

ResizeHandle::ResizeHandle(HandleType type, QGraphicsItem* parent)
    : QGraphicsRectItem(-kHandleSize / 2, -kHandleSize / 2, kHandleSize, kHandleSize, parent)
    , m_type(type)
{
    setFlags(QGraphicsItem::ItemIsMovable |
        QGraphicsItem::ItemSendsScenePositionChanges |
        QGraphicsItem::ItemIgnoresTransformations);
    setZValue(0);

    QBrush brush(kHandleColor);
    QPen pen(kHandleBorderColor, 1.0);
    pen.setCosmetic(true);
    setBrush(brush);
    setPen(pen);

    switch (type) {
    case HandleType::TopLeft:
    case HandleType::BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case HandleType::TopRight:
    case HandleType::BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    }
}

void ResizeHandle::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) 
    {
        m_isDragging = true;
        m_startPos = event->scenePos();
        event->accept();
    }
    else 
    {
        event->ignore();
    }
}

void ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_isDragging)
    {
        QPointF delta = event->scenePos() - m_startPos;
        m_startPos = event->scenePos();
        emit dragMove(m_type, delta);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void ResizeHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_isDragging && event->button() == Qt::LeftButton)
    {
        m_isDragging = false;
        emit dragRelease(m_type, m_startPos);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
