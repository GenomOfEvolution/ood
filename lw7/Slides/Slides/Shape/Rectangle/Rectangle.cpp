#include "Rectangle.h"
#include <algorithm>

CRectangle::CRectangle(PointD leftTop, double width, double height)
	: m_leftTop(leftTop)
	, m_width(width)
	, m_height(height)
{
}

std::optional<RectD> CRectangle::GetFrame() const
{
	return { {
		.left = m_leftTop.x,
		.top = m_leftTop.y,
		.width = m_width,
		.height = m_height
	} };
}

void CRectangle::SetFrame(const RectD& rect)
{
	m_leftTop = { rect.left, rect.top };
	m_width = rect.width;
	m_height = rect.height;
}

void CRectangle::Draw(ICanvas& canvas) const
{
	canvas.SetFillColor(GetFillColor());
	canvas.SetLineColor(GetOutlineColor());
	canvas.SetLineThickness(GetOutlineThickness());

	canvas.DrawPolygon({
		m_leftTop,
		{ m_leftTop.x + m_width, m_leftTop.y },
		{ m_leftTop.x + m_width, m_leftTop.y + m_height },
		{ m_leftTop.x, m_leftTop.y + m_height },
	});
}
