#pragma once
#include <qcolor.h>
#include <qpen.h>
#include <QGraphicsRectItem>

class SelectionBoxItem : public QGraphicsRectItem
{
public:
    explicit SelectionBoxItem(QGraphicsItem* parent = nullptr);

    void SetTargetItem(QGraphicsItem* item);
    void UpdateGeometry();

private:
    QGraphicsItem* m_targetItem = nullptr;

    static constexpr qreal kHandleSize = 8.0;
    static constexpr Qt::PenStyle kBorderStyle = Qt::DashLine;
    static inline const QColor kBorderColor = Qt::blue;
    static constexpr qreal kBorderWidth = 3;
};