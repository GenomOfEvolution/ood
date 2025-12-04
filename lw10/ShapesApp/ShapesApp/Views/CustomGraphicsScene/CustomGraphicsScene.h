#pragma once
#include <QGraphicsScene>
#include "../../Controllers/DocumentController/DocumentController.h"

class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomGraphicsScene(DocumentController* controller, QObject* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    DocumentController* m_controller;
    bool m_leftButtonPressed = false;
};