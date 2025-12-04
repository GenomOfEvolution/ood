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
    connect(m_controller, &DocumentController::itemRemoved, this, &WorkspaceWidget::HandleItemRemoved);
    connect(m_controller, &DocumentController::itemsMoved, this, &WorkspaceWidget::HandleItemsMoved);
    connect(m_controller, &DocumentController::documentLoaded, this, &WorkspaceWidget::HandleDocumentLoaded);

    connect(m_controller, &DocumentController::selectionChanged,
        this, &WorkspaceWidget::HandleSelectionChanged);

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

void WorkspaceWidget::HandleItemRemoved(int index)
{
    QGraphicsItem* item = FindSceneItemByIndex(static_cast<size_t>(index));
    if (item) 
    {
        m_scene->removeItem(item);
        delete item;
    }
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