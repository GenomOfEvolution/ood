#pragma once
#include "../Shape.h"
#include <array>

class CTriangle : public Shape
{
public:
	CTriangle(const PointD& p1, const PointD& p2, const PointD& p3);

	std::optional<RectD> GetFrame() const override;
	void SetFrame(const RectD& rect) override;

	void Draw(ICanvas& canvas) const override;

private:
	void ScalePoint(PointD& point,
		const RectD& initialFrame, const RectD& newFrame,
		double scaleX, double scaleY);

	std::array<PointD, 3> m_points;
};