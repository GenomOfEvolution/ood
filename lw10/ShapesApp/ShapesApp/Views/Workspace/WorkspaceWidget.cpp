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
    , m_factory(m_controller->GetImageStorage())
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
    connect(m_controller, &DocumentController::itemsMoved, this, &WorkspaceWidget::HandleItemsMoved);
    connect(m_controller, &DocumentController::documentLoaded, this, &WorkspaceWidget::HandleDocumentLoaded);

    connect(m_controller, &DocumentController::deleteLastItem, this, &WorkspaceWidget::HandleDeleteLastItem);

    connect(m_controller, &DocumentController::selectionChanged,
        this, &WorkspaceWidget::HandleSelectionChanged);

    connect(m_controller, &DocumentController::documentChanged,
        this, &WorkspaceWidget::HandleDocumentChanged);

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

    QPointF sceneDelta = m_view->mapToScene(QPoint(dx * m_scale, dy * m_scale)) - m_view->mapToScene(QPoint(0, 0));

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

void WorkspaceWidget::HandleDocumentChanged()
{
    ClearSelectionBoxes();
    m_scene->clear();

    for (size_t i = 0; i < m_controller->GetItemsCount(); ++i) 
    {
        auto item = m_controller->GetItemAtIndex(i);
        auto preview = item->GetPreview();
        preview.m_index = i;

        HandleItemAdded(preview);
    }

    UpdateSelectionBoxes();
    FitSceneToView();
}

void WorkspaceWidget::HandleDeleteLastItem(size_t index)
{
    auto item = FindSceneItemByIndex(index);
    if (item)
        m_scene->removeItem(item);
    UpdateSelectionBoxes();
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

    m_scale = scale;
}