#include "CRectangle.h"
#include <QPainterPath>
#include <QTransform>

CRectangle::CRectangle(const QPointF& leftTop, float width, float height)
    : m_rect(leftTop.x(), leftTop.y(), width, height)
{
}

CRectangle::CRectangle(const QPointF& leftTop, const QPoint& rightBottom)
    : m_rect(QRectF(leftTop, rightBottom))
{
}

bool CRectangle::ContainsPoint(const QPointF& point) const
{
    return m_rect.contains(point);
}

QRectF CRectangle::GetBoundingBox() const
{
    return m_rect;
}

void CRectangle::MoveBy(const QPointF& delta)
{
    m_rect.translate(delta);
}

void CRectangle::ApplyTransform(const QTransform& transform)
{
    QPainterPath path;
    path.addRect(m_rect);

    path = transform.map(path);

    m_rect = path.boundingRect();
}