#pragma once
#include "../../Controllers/DocumentController/DocumentController.h"
#include "../Factory/QtGraphicsItemFactory.h"
#include "../CustomGraphicsScene/CustomGraphicsScene.h"

#include <qwidget.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <qpoint.h>

const int DocumentIndexRole = Qt::UserRole + 1;

class WorkspaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceWidget(DocumentController* controller, QWidget* parent = nullptr);
    ~WorkspaceWidget() override;

    QGraphicsScene* scene() const { return m_scene; }
    QGraphicsView* view() const { return m_view; }

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void HandleItemsMoved(std::vector<size_t> indexes, double dx, double dy);
    void HandleItemAdded(const DocItemPreview& preview);
    void HandleItemRemoved(int index);
    void HandleDocumentLoaded();

private:
    QGraphicsItem* FindSceneItemByIndex(size_t index) const;
    void SetupView();
    void FitSceneToView();

    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QRectF m_sceneBoundary;

    QPointF m_lastCenter;

    DocumentController* m_controller;
    QtGraphicsItemFactory m_factory;

    bool m_isLeftButtonDown = false;
};