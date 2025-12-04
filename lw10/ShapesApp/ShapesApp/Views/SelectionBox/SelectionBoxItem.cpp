#include "SelectionBoxItem.h"

SelectionBoxItem::SelectionBoxItem(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{
    setFlags(ItemIgnoresParentOpacity);
    setZValue(0);

    QPen pen(kBorderColor, kBorderWidth, kBorderStyle);
    pen.setCosmetic(true); 
    setPen(pen);
    setBrush(Qt::NoBrush);
}

void SelectionBoxItem::SetTargetItem(QGraphicsItem* item)
{
    m_targetItem = item;
    UpdateGeometry();
}

void SelectionBoxItem::UpdateGeometry()
{
    if (!m_targetItem) 
        return;

    QRectF sceneRect = m_targetItem->sceneBoundingRect();

    QRectF rect = mapFromScene(sceneRect).boundingRect();
    setRect(rect);
}