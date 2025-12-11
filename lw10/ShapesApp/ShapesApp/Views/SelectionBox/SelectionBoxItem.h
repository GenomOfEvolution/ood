#pragma once
#include "../../Models/Shared/HandleType.h"
#include <qcolor.h>
#include <qpen.h>
#include <QGraphicsRectItem>

class ResizeHandle : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit ResizeHandle(HandleType type, QGraphicsItem* parent = nullptr);

    HandleType GetType() const { return m_type; }

signals:
    void dragMove(HandleType type, QPointF point);
    void dragRelease(HandleType type, QPointF point);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    HandleType m_type;
    QPointF m_startPos;
    bool m_isDragging = false;

    static constexpr qreal kHandleSize = 8.0;
    static constexpr Qt::PenStyle kBorderStyle = Qt::DashLine;
    static inline const QColor kHandleColor = Qt::white;
    static inline const QColor kHandleBorderColor = Qt::blue;
};

class SelectionBoxItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit SelectionBoxItem(QGraphicsItem* parent = nullptr);

    void SetTargetItem(QGraphicsItem* item);
    void UpdateGeometry();

    void HandleHandleMove(HandleType type, const QPointF& delta);
    void HandleResizeMove(const QRectF& newBoundingBox);

signals:
    void resizeRequested(HandleType type, double dx, double dy);

private:
    void CreateHandles();
    void PositionHandles();

    void ResizeImageItem(QGraphicsPixmapItem* image, const QRectF& newBox);
    void ResizeRectItem(QGraphicsRectItem* rect, const QRectF& newBox);
    void ResizeTriangleItem(QGraphicsPolygonItem* triangle, const QRectF& newBox);
    void ResizeEllipseItem(QGraphicsEllipseItem* ellipse, const QRectF& newBox);

    QGraphicsItem* m_targetItem = nullptr;

    static constexpr qreal kHandleSize = 8.0;
    static constexpr Qt::PenStyle kBorderStyle = Qt::DashLine;
    static inline const QColor kBorderColor = Qt::blue;
    static constexpr qreal kBorderWidth = 3;

    std::array<ResizeHandle*, 4> m_handles;
};