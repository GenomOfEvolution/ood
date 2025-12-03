#include "CEllipse.h"
#include <cmath>
#include <algorithm>

CEllipse::CEllipse(const Point& center, double verticalRadius, double horizontalRadius)
    : m_center(center), m_vertRadius(verticalRadius), m_horizRadius(horizontalRadius)
{
}

Point CEllipse::GetCenter() const
{
    return m_center;
}

double CEllipse::GetVertRadius() const
{
    return m_vertRadius;
}

double CEllipse::GetHorizRadius() const
{
    return m_horizRadius;
}

bool CEllipse::ContainsPoint(const Point& point) const
{
    if (std::abs(m_horizRadius) < 1e-6 || std::abs(m_vertRadius) < 1e-6) 
    {
        return false;
    }

    double dx = point.x - m_center.x;
    double dy = point.y - m_center.y;

    double normalizedX = dx / m_horizRadius;
    double normalizedY = dy / m_vertRadius;

    return (normalizedX * normalizedX + normalizedY * normalizedY) <= 1.0 + 1e-6;
}

Rect CEllipse::GetBoundingBox() const
{
    double absHoriz = std::abs(m_horizRadius);
    double absVert = std::abs(m_vertRadius);

    double left = m_center.x - absHoriz;
    double top = m_center.y - absVert;
    double width = 2.0 * absHoriz;
    double height = 2.0 * absVert;

    return Rect(left, top, width, height);
}

void CEllipse::MoveBy(const Point& delta)
{
    m_center += delta;
}

void CEllipse::Resize(const Rect& newBoundingBox)
{
    m_center.x = newBoundingBox.x + newBoundingBox.width / 2.0;
    m_center.y = newBoundingBox.y + newBoundingBox.height / 2.0;

    m_horizRadius = newBoundingBox.width / 2.0;
    m_vertRadius = newBoundingBox.height / 2.0;

    if (std::abs(m_horizRadius) < 1e-6) m_horizRadius = 1e-6 * (newBoundingBox.width >= 0 ? 1 : -1);
    if (std::abs(m_vertRadius) < 1e-6) m_vertRadius = 1e-6 * (newBoundingBox.height >= 0 ? 1 : -1);
}

DocItemPreview CEllipse::GetPreview() const
{
    return DocItemPreview
    {
        .m_type = DocItemPreview::ItemType::Ellipse,
        .m_points = { m_center },
        .m_boundingBox = this->GetBoundingBox()
    };;
}
