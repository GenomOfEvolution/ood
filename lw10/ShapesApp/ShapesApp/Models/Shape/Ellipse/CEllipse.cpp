#include "CEllipse.h"
#include <QPainterPath>
#include <QTransform>
#include <cmath>

CEllipse::CEllipse(const QPointF& center, float verticalRadius, float horizontalRadius)
    : m_center(center), m_vertRadius(verticalRadius), m_horizRadius(horizontalRadius)
{
}

bool CEllipse::ContainsPoint(const QPointF& point) const
{
    float dx = point.x() - m_center.x();
    float dy = point.y() - m_center.y();

    if (m_horizRadius <= 0 || m_vertRadius <= 0) 
    {
        return false;
    }

    float normalizedX = dx / m_horizRadius;
    float normalizedY = dy / m_vertRadius;

    return (normalizedX * normalizedX + normalizedY * normalizedY) <= 1.0f;
}

QRectF CEllipse::GetBoundingBox() const
{
    float left = m_center.x() - m_horizRadius;
    float top = m_center.y() - m_vertRadius;
    float width = 2.0f * m_horizRadius;
    float height = 2.0f * m_vertRadius;

    return QRectF(left, top, width, height);
}

void CEllipse::MoveBy(const QPointF& delta)
{
    m_center += delta;
}

void CEllipse::ApplyTransform(const QTransform& transform)
{
    QPainterPath path;
    path.addEllipse(GetBoundingBox());

    path = transform.map(path);

    QRectF newRect = path.boundingRect();

    m_center = newRect.center();
    m_horizRadius = newRect.width() / 2.0f;
    m_vertRadius = newRect.height() / 2.0f;
}