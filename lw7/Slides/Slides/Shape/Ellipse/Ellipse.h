#pragma once
#include "../Shape.h"

class Ellipse : public Shape
{
public:
	Ellipse(const PointD& center, double horizontalRadius, double vertRadius);

	std::optional<RectD> GetFrame() const override;
	void SetFrame(const RectD& rect) override;

	void Draw(ICanvas& canvas) const override;

private:
	PointD m_center;
	double m_hRadius, m_vRadius;
};