#pragma once
#include "../../Controllers/DocumentController/DocumentController.h"
#include "../Factory/QtGraphicsItemFactory.h"

#include <qwidget.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <qpoint.h>

class WorkspaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceWidget(DocumentController* controller, QWidget* parent = nullptr);
    ~WorkspaceWidget() override;

    QGraphicsScene* scene() const { return m_scene; }
    QGraphicsView* view() const { return m_view; }


    //void handleItemResized(int index, Rect boundingBox);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void HandleItemAdded(const std::string& itemName);

    void SetupView();
    void FitSceneToView();

    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    QPointF m_lastCenter;

    DocumentController* m_controller;
    QtGraphicsItemFactory m_factory;
};