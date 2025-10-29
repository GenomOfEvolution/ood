#include "Rectangle.h"
#include <algorithm>

Rectangle::Rectangle(PointD leftTop, double width, double height)
	: m_leftTop(leftTop)
	, m_width(width)
	, m_height(height)
{
}

std::optional<RectD> Rectangle::GetFrame() const
{
	return { {
		.left = m_leftTop.x,
		.top = m_leftTop.y,
		.width = m_width,
		.height = m_height
	} };
}

void Rectangle::SetFrame(const RectD& rect)
{
	m_leftTop = { rect.left, rect.top };
	m_width = rect.width;
	m_height = rect.height;
}

void Rectangle::Draw(ICanvas& canvas) const
{
}
