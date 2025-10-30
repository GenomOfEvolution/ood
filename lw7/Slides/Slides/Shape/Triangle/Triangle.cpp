#include "Triangle.h"
#include <algorithm>

CTriangle::CTriangle(const PointD& p1, const PointD& p2, const PointD& p3)
	: m_points({p1, p2, p3})
{
}

std::optional<RectD> CTriangle::GetFrame() const
{
	const auto& [minX, maxX] = std::minmax({ m_points[0].x, m_points[1].x, m_points[2].x });
	const auto& [minY, maxY] = std::minmax({ m_points[0].y, m_points[1].y, m_points[2].y });

	return { {
		.left = minX,
		.top = minY,
		.width = maxX - minX,
		.height = maxY - minY,
	} };
}

void CTriangle::SetFrame(const RectD& rect)
{
	const auto curFrame = GetFrame().value();

	const auto scaleFactorX = rect.width / curFrame.width;
	const auto scaleFactorY = rect.height / curFrame.height;

	ScalePoint(m_points[0], curFrame, rect, scaleFactorX, scaleFactorY);
	ScalePoint(m_points[1], curFrame, rect, scaleFactorX, scaleFactorY);
	ScalePoint(m_points[2], curFrame, rect, scaleFactorX, scaleFactorY);
}

void CTriangle::Draw(ICanvas& canvas) const
{
	canvas.SetFillColor(GetFillColor());
	canvas.SetLineColor(GetOutlineColor());
	canvas.SetLineThickness(GetOutlineThickness());

	canvas.DrawPolygon({ m_points[0], m_points[1], m_points[2] });
}

void CTriangle::ScalePoint(PointD& point,
	const RectD& initialFrame, const RectD& newFrame,
	double scaleX, double scaleY)
{
	point.x = newFrame.left + (point.x - initialFrame.left) * scaleX;
	point.y = newFrame.top + (point.y - initialFrame.top) * scaleY;
}