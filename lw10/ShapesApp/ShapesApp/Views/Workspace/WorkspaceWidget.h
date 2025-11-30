#pragma once
#include <qwidget.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <qpoint.h>

class WorkspaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceWidget(QWidget* parent = nullptr);
    ~WorkspaceWidget() override;

    QGraphicsScene* scene() const { return m_scene; }
    QGraphicsView* view() const { return m_view; }

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void SetupView();
    void FitSceneToView();

    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    QPointF m_lastCenter;
    qreal m_currentScale = 1.0;
    const qreal MIN_SCALE = 0.0001;
    const qreal MAX_SCALE = 10000.0;
};