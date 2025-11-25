#include "CTriangle.h"
#include <algorithm>
#include <QPainterPath>
#include <QTransform>

CTriangle::CTriangle(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
    m_points = { p1, p2, p3 };
}

CTriangle::CTriangle(const std::array<QPointF, 3>& points)
    : m_points(points)
{
}

bool CTriangle::ContainsPoint(const QPointF& point) const
{
    const QPointF& p0 = m_points[0];
    const QPointF& p1 = m_points[1];
    const QPointF& p2 = m_points[2];

    QPointF v0 = p2 - p0;
    QPointF v1 = p1 - p0;
    QPointF v2 = point - p0;

    float dot00 = v0.x() * v0.x() + v0.y() * v0.y();
    float dot01 = v0.x() * v1.x() + v0.y() * v1.y();
    float dot02 = v0.x() * v2.x() + v0.y() * v2.y();
    float dot11 = v1.x() * v1.x() + v1.y() * v1.y();
    float dot12 = v1.x() * v2.x() + v1.y() * v2.y();

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

QRectF CTriangle::GetBoundingBox() const
{
    double minX = std::min({ m_points[0].x(), m_points[1].x(), m_points[2].x() });
    double minY = std::min({ m_points[0].y(), m_points[1].y(), m_points[2].y() });
    double maxX = std::max({ m_points[0].x(), m_points[1].x(), m_points[2].x() });
    double maxY = std::max({ m_points[0].y(), m_points[1].y(), m_points[2].y() });

    return QRectF(QPointF(minX, minY), QPointF(maxX, maxY));
}

void CTriangle::MoveBy(const QPointF& delta)
{
    for (auto& point : m_points) 
    {
        point += delta;
    }
}

void CTriangle::ApplyTransform(const QTransform& transform)
{
    for (auto& point : m_points) 
    {
        point = transform.map(point);
    }
}
