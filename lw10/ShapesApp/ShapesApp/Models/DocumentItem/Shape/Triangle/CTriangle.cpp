#include "CTriangle.h"
#include <algorithm>
#include <cmath>

CTriangle::CTriangle(const Point& p1, const Point& p2, const Point& p3)
{
    m_points = { p1, p2, p3 };
}

CTriangle::CTriangle(const std::array<Point, 3>& points)
    : m_points(points)
{
}

bool CTriangle::ContainsPoint(const Point& point) const
{
    const Point& p0 = m_points[0];
    const Point& p1 = m_points[1];
    const Point& p2 = m_points[2];

    Point v0 = { p2.x - p0.x, p2.y - p0.y };
    Point v1 = { p1.x - p0.x, p1.y - p0.y };
    Point v2 = { point.x - p0.x, point.y - p0.y };

    double dot00 = v0.x * v0.x + v0.y * v0.y;
    double dot01 = v0.x * v1.x + v0.y * v1.y;
    double dot02 = v0.x * v2.x + v0.y * v2.y;
    double dot11 = v1.x * v1.x + v1.y * v1.y;
    double dot12 = v1.x * v2.x + v1.y * v2.y;

    double invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

Rect CTriangle::GetBoundingBox() const
{
    double minX = std::min({ m_points[0].x, m_points[1].x, m_points[2].x });
    double minY = std::min({ m_points[0].y, m_points[1].y, m_points[2].y });
    double maxX = std::max({ m_points[0].x, m_points[1].x, m_points[2].x });
    double maxY = std::max({ m_points[0].y, m_points[1].y, m_points[2].y });

    return Rect(minX, minY, maxX - minX, maxY - minY);
}

void CTriangle::MoveBy(const Point& delta)
{
    for (auto& point : m_points)
    {
        point += delta;
    }
}

void CTriangle::Resize(const Rect& newBoundingBox)
{
    Rect currentBox = GetBoundingBox();
    double currentWidth = currentBox.GetWidth();
    double currentHeight = currentBox.GetHeight();

    if (currentWidth == 0) currentWidth = 1.0;
    if (currentHeight == 0) currentHeight = 1.0;

    bool flipX = (newBoundingBox.GetWidth() < 0);
    bool flipY = (newBoundingBox.GetHeight() < 0);

    double newLeft = flipX ?
        newBoundingBox.x + newBoundingBox.GetWidth() :
        newBoundingBox.x;

    double newTop = flipY ?
        newBoundingBox.y + newBoundingBox.GetHeight() :
        newBoundingBox.y;

    double newWidth = std::abs(newBoundingBox.GetWidth());
    double newHeight = std::abs(newBoundingBox.GetHeight());

    for (auto& point : m_points)
    {
        double relX = (point.x - currentBox.x) / currentWidth;
        double relY = (point.y - currentBox.y) / currentHeight;

        if (flipX) relX = 1.0 - relX;
        if (flipY) relY = 1.0 - relY;

        point.x = newLeft + relX * newWidth;
        point.y = newTop + relY * newHeight;
    }
}