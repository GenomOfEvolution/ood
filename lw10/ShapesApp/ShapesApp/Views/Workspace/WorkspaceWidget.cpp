#include "WorkspaceWidget.h"
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>
#include <QScrollBar>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

#include <iostream>

WorkspaceWidget::WorkspaceWidget(
    DocumentController* controller,
    QWidget* parent)
    : QWidget(parent)
    , m_controller(controller)
    , m_lastCenter(0, 0)
{
    m_scene = new CustomGraphicsScene(m_controller, this);
    m_scene->setSceneRect(0, 0, 1200, 500);

    m_view = new QGraphicsView(m_scene, this);
    SetupView();

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_view);
    setLayout(layout);

    connect(m_controller, &DocumentController::itemAdded, this, &WorkspaceWidget::HandleItemAdded);
    connect(m_controller, &DocumentController::itemsRemoved, this, &WorkspaceWidget::HandleItemRemoved);
    connect(m_controller, &DocumentController::itemsMoved, this, &WorkspaceWidget::HandleItemsMoved);
    connect(m_controller, &DocumentController::documentLoaded, this, &WorkspaceWidget::HandleDocumentLoaded);

    connect(m_controller, &DocumentController::selectionChanged,
        this, &WorkspaceWidget::HandleSelectionChanged);

    connect(m_controller, &DocumentController::itemsResized, this, &WorkspaceWidget::HandleItemsResized);

    QTimer::singleShot(0, this, &WorkspaceWidget::FitSceneToView);
    m_view->installEventFilter(this);
}

void WorkspaceWidget::HandleItemsMoved(std::vector<size_t> indexes, double dx, double dy)
{
    m_scene->blockSignals(true);
    QPointF delta(dx, dy);
    bool sceneUpdated = false;

    for (auto index : indexes) 
    {
        QGraphicsItem* item = FindSceneItemByIndex(index);
        if (item) 
        {
            item->setPos(item->pos() + delta);
        }
    }

    UpdateSelectionBoxes();

    m_scene->blockSignals(false);
    m_scene->update();
}

void WorkspaceWidget::HandleItemAdded(const DocItemPreview& itemName)
{
    auto item = m_factory.CreateItem(itemName).release();
    
    item->setData(DocumentIndexRole, static_cast<qint64>(itemName.m_index));

    m_scene->addItem(item);
}


struct ItemData {
    int type;
    QPointF pos;
    qreal rotation;
    qreal scaleX;
    qreal scaleY;
    int zValue;
    QVariant userData;
    QRectF boundingRect;

    // Для фигур
    QRectF rect;
    QBrush brush;
    QPen pen;
    QPolygonF polygon;

    // Для изображений
    QPixmap pixmap;
    QPointF offset;
    Qt::TransformationMode transformationMode;
};

void WorkspaceWidget::HandleItemRemoved(std::vector<size_t> indexes)
{
    ClearSelectionBoxes();
    std::sort(indexes.begin(), indexes.end(), std::greater<size_t>());

    // 2. Собираем элементы для удаления
    std::vector<QGraphicsItem*> itemsToDelete;
    for (auto index : indexes) {
        QGraphicsItem* item = FindSceneItemByIndex(index);
        if (item) {
            itemsToDelete.push_back(item);
        }
    }

    // 3. Сохраняем параметры вида
    QTransform transform = m_view->transform();
    QPointF centerPoint = m_view->mapToScene(m_view->viewport()->rect().center());

    // 4. Создаем клоны для сохранения
    std::vector<std::unique_ptr<QGraphicsItem>> savedClones;

    for (QGraphicsItem* item : m_scene->items()) {
        // Проверяем, нужно ли удалить этот элемент
        bool shouldDelete = false;
        for (QGraphicsItem* delItem : itemsToDelete) {
            if (item == delItem) {
                shouldDelete = true;
                break;
            }
        }

        if (!shouldDelete) 
        {
            if (auto clone = m_factory.Clone(item)) 
            {
                savedClones.push_back(std::move(clone));
            }
        }
    }

    qDeleteAll(m_scene->items());
    m_scene->clear();

    int index = 0;
    for (auto& clone : savedClones) 
    {
        clone->setFlags(QGraphicsItem::ItemIsMovable);
        clone->setData(DocumentIndexRole, static_cast<qint64>(index));
        m_scene->addItem(clone.release());
    }

    // 7. Восстанавливаем вид
    m_view->setTransform(transform);
    m_view->centerOn(centerPoint);

    UpdateSelectionBoxes();
}

void WorkspaceWidget::HandleItemsResized(const std::vector<QRectF>& newBoundingBoxes)
{
    if (m_selectionBoxes.size() != newBoundingBoxes.size())
    {
        UpdateSelectionBoxes();
        return;
    }

    for (size_t i = 0; i < m_selectionBoxes.size(); ++i) 
    {
        SelectionBoxItem* box = m_selectionBoxes[i];
        if (i < newBoundingBoxes.size()) 
        {
            box->HandleResizeMove(newBoundingBoxes[i]);
        }
    }
}

void WorkspaceWidget::HandleResizeRequested(HandleType type, qreal dx, qreal dy)
{
    auto selectedIndexes = m_controller->GetSelectedIndexes();
    if (selectedIndexes.empty()) 
        return;

    QPointF sceneDelta = m_view->mapToScene(QPoint(dx, dy)) - m_view->mapToScene(QPoint(0, 0));

    m_controller->Resize(type, sceneDelta.x(), sceneDelta.y());
}

void WorkspaceWidget::HandleResizeFinished()
{
}

void WorkspaceWidget::HandleSelectionChanged()
{
    if (m_updatingSelection) 
        return;
    UpdateSelectionBoxes();
}

void WorkspaceWidget::UpdateSelectionBoxes()
{
    m_updatingSelection = true;
    ClearSelectionBoxes();

    auto selectedIndexes = m_controller->GetSelectedIndexes();

    for (size_t index : selectedIndexes) 
    {
        QGraphicsItem* item = FindSceneItemByIndex(index);
        if (item) 
        {
            SelectionBoxItem* box = new SelectionBoxItem();
            box->SetTargetItem(item);

            connect(box, &SelectionBoxItem::resizeRequested,
                this, &WorkspaceWidget::HandleResizeRequested);

            connect(box, &SelectionBoxItem::resizeFinished,
                this, &WorkspaceWidget::HandleResizeFinished);

            m_scene->addItem(box);
            m_selectionBoxes.push_back(box);
        }
    }

    m_updatingSelection = false;
}

void WorkspaceWidget::HandleDocumentLoaded()
{
    m_scene->clear();
}

QGraphicsItem* WorkspaceWidget::FindSceneItemByIndex(size_t index) const
{
    for (QGraphicsItem* item : m_scene->items()) 
    {
        if (item->data(DocumentIndexRole).isValid()) 
        {
            qint64 storedIndex = item->data(DocumentIndexRole).toLongLong();
            if (static_cast<size_t>(storedIndex) == index) 
            {
                return item;
            }
        }
    }
    return nullptr;
}

void WorkspaceWidget::ClearSelectionBoxes()
{
    for (SelectionBoxItem* box : m_selectionBoxes) 
    {
        m_scene->removeItem(box);
        delete box;
    }

    m_selectionBoxes.clear();
}

void WorkspaceWidget::SetupView()
{
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    m_view->setInteractive(true);

    m_view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_view->setTransformationAnchor(QGraphicsView::NoAnchor);
    m_view->setResizeAnchor(QGraphicsView::NoAnchor);
}

WorkspaceWidget::~WorkspaceWidget() = default;

void WorkspaceWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    FitSceneToView();
    UpdateSelectionBoxes();
}

void WorkspaceWidget::FitSceneToView()
{
    if (!m_view->scene() || m_view->viewport()->width() <= 0 || m_view->viewport()->height() <= 0)
        return;

    QRectF sceneRect = m_scene->sceneRect();
    double scaleX = m_view->viewport()->width() / sceneRect.width();
    double scaleY = m_view->viewport()->height() / sceneRect.height();
    double scale = qMin(scaleX, scaleY);

    QTransform transform;
    transform.scale(scale, scale);
    m_view->setTransform(transform);
}