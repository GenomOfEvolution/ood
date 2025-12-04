#include "CRectangle.h"
#include <algorithm>
#include <cmath>

CRectangle::CRectangle(const Point& leftTop, double width, double height)
    : m_rect{ leftTop.x, leftTop.y, width, height }
{
}

CRectangle::CRectangle(const Point& leftTop, const Point& rightBottom)
    : m_rect{ leftTop.x, leftTop.y, rightBottom.x - leftTop.x, rightBottom.y - leftTop.y }
{
}

bool CRectangle::ContainsPoint(const Point& point) const
{
    double left = std::min(m_rect.x, m_rect.x + m_rect.width);
    double right = std::max(m_rect.x, m_rect.x + m_rect.width);
    double top = std::min(m_rect.y, m_rect.y + m_rect.height);
    double bottom = std::max(m_rect.y, m_rect.y + m_rect.height);

    return (point.x >= left && point.x <= right) &&
        (point.y >= top && point.y <= bottom) || this->GetBoundingBox().ContainsPoint(point);
}

Rect CRectangle::GetBoundingBox() const
{
    double left = std::min(m_rect.x, m_rect.x + m_rect.width);
    double top = std::min(m_rect.y, m_rect.y + m_rect.height);
    double width = std::abs(m_rect.width);
    double height = std::abs(m_rect.height);

    return { left, top, width, height };
}

void CRectangle::MoveBy(const Point& delta)
{
    m_rect.x += delta.x;
    m_rect.y += delta.y;
}

void CRectangle::Resize(const Rect& newBoundingBox)
{
    m_rect = newBoundingBox;
}

DocItemPreview CRectangle::GetPreview() const
{
    return DocItemPreview
    {
        .m_type = DocItemPreview::ItemType::Rectangle,
        .m_boundingBox = m_rect
    };
}
