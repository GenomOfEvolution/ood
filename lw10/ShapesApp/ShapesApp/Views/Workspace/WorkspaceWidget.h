#pragma once
#include "../../Controllers/DocumentController/DocumentController.h"
#include "../Factory/QtGraphicsItemFactory.h"
#include "../CustomGraphicsScene/CustomGraphicsScene.h"
#include "../SelectionBox/SelectionBoxItem.h"

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
    void HandleItemRemoved(std::vector<size_t> indexes);

    void HandleItemsResized(const std::vector<QRectF>& newBoundingBoxes);
    void HandleResizeRequested(HandleType type, qreal dx, qreal dy);
    void HandleResizeFinished();

    void HandleSelectionChanged();
    void UpdateSelectionBoxes();

    void HandleDocumentLoaded();

private:
    QGraphicsItem* FindSceneItemByIndex(size_t index) const;
    void ClearSelectionBoxes();
    void SetupView();
    void FitSceneToView();

    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QRectF m_sceneBoundary;

    QPointF m_lastCenter;
    float m_scale = 1.0f;

    DocumentController* m_controller;
    QtGraphicsItemFactory m_factory;

    bool m_isLeftButtonDown = false;
    bool m_updatingSelection = false;
    std::vector<SelectionBoxItem*> m_selectionBoxes;
};