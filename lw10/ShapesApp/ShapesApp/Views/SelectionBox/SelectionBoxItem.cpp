#include "SelectionBoxItem.h"
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

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

        if (m_targetItem) {
            m_targetItem->setTransform(QTransform().scale(
                newBoundingBox.width() / m_targetItem->boundingRect().width(),
                newBoundingBox.height() / m_targetItem->boundingRect().height()
            ));
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
