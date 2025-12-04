#include "CustomGraphicsScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

CustomGraphicsScene::CustomGraphicsScene(DocumentController* controller, QObject* parent)
    : QGraphicsScene(parent)
    , m_controller(controller)
{
}

void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_leftButtonPressed = true;
        m_controller->handleMousePress(event->scenePos(), event->modifiers());
    }
    QGraphicsScene::mousePressEvent(event); 
}

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_leftButtonPressed) {
        m_controller->handleMouseMove(event->scenePos(), event->modifiers());
    }
    QGraphicsScene::mouseMoveEvent(event); 
}

void CustomGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_leftButtonPressed) 
    {
        m_leftButtonPressed = false;
        m_controller->handleMouseRelease(event->scenePos(), event->modifiers());
    }
    QGraphicsScene::mouseReleaseEvent(event); 
}