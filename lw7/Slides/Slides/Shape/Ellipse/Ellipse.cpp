#include "Ellipse.h"

CEllipse::CEllipse(const PointD& center, double horizontalRadius, double vertRadius)
    : m_center(center)
    , m_hRadius(horizontalRadius)
    , m_vRadius(vertRadius)
{
}

std::optional<RectD> CEllipse::GetFrame() const
{
	return { {
		.left = m_center.x - m_hRadius,
		.top = m_center.y - m_vRadius,
		.width = m_hRadius * 2,
		.height = m_vRadius * 2,
	} };
}

void CEllipse::SetFrame(const RectD& rect)
{
	m_center = { rect.left + rect.width / 2, rect.top + rect.height / 2 };
	m_hRadius = rect.width / 2;
	m_vRadius = rect.height / 2;
}

void CEllipse::Draw(ICanvas& canvas) const
{
	canvas.SetFillColor(GetFillColor());
	canvas.SetLineColor(GetOutlineColor());
	canvas.SetLineThickness(GetOutlineThickness());

	canvas.DrawEllipse(m_center, m_hRadius, m_vRadius);
}
