#pragma once
#include "../Shape.h"

class CRectangle : public Shape
{
public:
	CRectangle(PointD leftTop, double width, double height);

	std::optional<RectD> GetFrame() const override;
	void SetFrame(const RectD& rect) override;

	void Draw(ICanvas& canvas) const override;

private:
	PointD m_leftTop;
	double m_width, m_height;
};