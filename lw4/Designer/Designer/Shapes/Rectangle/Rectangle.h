#pragma once
#include "../Shape.h"
#include "../ShapeTypes.h"

class CRectangle : public Shape
{
public:
	CRectangle(Color color, Point leftTopCorner, double width, double height);

	Point GetLeftTop() const { return m_leftTop; };
	Point GetRightBottom() const { return Point{ .x = m_leftTop.x + m_width, .y = m_leftTop.y + m_height }; };

	void Draw(ICanvas& canvas) const override;

private:
	Point m_leftTop;
	double m_width, m_height;
};