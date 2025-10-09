#include "Triangle.h"

Triangle::Triangle(Color color, Point p1, Point p2, Point p3)
	: Shape(color)
	, m_p1(p1)
	, m_p2(p2)
	, m_p3(p3)
{
}

void Triangle::Draw(ICanvas& canvas) const
{
	canvas.SetColor(GetColor());

	canvas.DrawLine(m_p1, m_p2);
	canvas.DrawLine(m_p2, m_p3);
	canvas.DrawLine(m_p3, m_p1);
}
