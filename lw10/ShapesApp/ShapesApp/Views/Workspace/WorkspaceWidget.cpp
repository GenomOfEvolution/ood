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
    m_scene = new QGraphicsScene(0, 0, 1200, 500, this);

    m_view = new QGraphicsView(m_scene, this);
    SetupView();

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_view);
    setLayout(layout);

    connect(controller, &DocumentController::itemAdded,
        this, &WorkspaceWidget::HandleItemAdded);

    connect(controller, &DocumentController::documentLoaded,
        this, &WorkspaceWidget::HandleDocumentLoaded);

    QTimer::singleShot(0, this, &WorkspaceWidget::FitSceneToView);
}

void WorkspaceWidget::HandleItemAdded(const DocItemPreview& itemName)
{
    auto item = m_factory.CreateItem(itemName).release();
    item->setFlags(QGraphicsItem::ItemIsMovable);
    m_scene->addItem(item);
}

void WorkspaceWidget::HandleDocumentLoaded()
{
    m_scene->clear();
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