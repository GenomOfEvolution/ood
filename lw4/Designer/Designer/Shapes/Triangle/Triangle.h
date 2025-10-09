#pragma once
#include "../Shape.h"
#include "../ShapeTypes.h"

class Triangle : public Shape
{
public:
	Triangle(Color color, Point p1, Point p2, Point p3);

	Point GetP1() const { return m_p1; };
	Point GetP2() const { return m_p2; };
	Point GetP3() const { return m_p3; };
	void Draw(ICanvas& canvas) const override;

private:
	Point m_p1, m_p2, m_p3;
};