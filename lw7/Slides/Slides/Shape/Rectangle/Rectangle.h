#pragma once
#include "../Shape.h"

class Rectangle : public Shape
{
public:
	Rectangle(PointD leftTop, double width, double height);

	std::optional<RectD> GetFrame() const override;
	void SetFrame(const RectD& rect) override;

	void Draw(ICanvas& canvas) const override;

private:
	PointD m_leftTop;
	double m_width, m_height;
};