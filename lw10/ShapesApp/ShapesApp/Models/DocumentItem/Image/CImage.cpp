#include "CImage.h"

CImage::CImage(const std::string& path, const Point& position, double width, double height)
    : m_path(path), m_boundingBox(position, width, height)
{
}

std::string CImage::GetPath() const
{
    return m_path;
}

void CImage::SetPath(const std::string& path)
{
    m_path = path;
}

bool CImage::ContainsPoint(const Point& point) const
{
    return m_boundingBox.ContainsPoint(point);
}

Rect CImage::GetBoundingBox() const
{
    return m_boundingBox.GetBoundingBox();
}

void CImage::MoveBy(const Point& delta)
{
    m_boundingBox.MoveBy(delta);
}

void CImage::Resize(const Rect& newBoundingBox)
{
    m_boundingBox.Resize(newBoundingBox);
}