#pragma once
#include "../Shape.h"
#include "../ShapeTypes.h"

class Ellipse : public Shape
{
public:
	Ellipse(Color color, Point center, double horizontalRadius, double verticalRadius);

	Point GetCenter() const { return m_center; };
	double GetHorizontalRadius() const { return m_horizontalRadius; };
	double GetVerticalRadius() const { return m_verticalRadius; };

	void Draw(ICanvas& canvas) const override;

private:
	Point m_center;
	double m_horizontalRadius;
	double m_verticalRadius;
};