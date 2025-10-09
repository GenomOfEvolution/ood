#include "Rectangle.h"

Rectangle::Rectangle(Color color, Point leftTopCorner, double width, double height)
	: Shape(color)
	, m_leftTop(leftTopCorner)
	, m_width(width)
	, m_height(height)
{
}

void Rectangle::Draw(ICanvas& canvas) const
{
	canvas.SetColor(GetColor());

	Point p1 = m_leftTop;
	Point p2 = { m_leftTop.x + m_width, m_leftTop.y };
	Point p3 = { m_leftTop.x + m_width, m_leftTop.y + m_height };
	Point p4 = { m_leftTop.x, m_leftTop.y + m_height };

	canvas.DrawLine(p1, p2); 
	canvas.DrawLine(p2, p3);
	canvas.DrawLine(p3, p4);
	canvas.DrawLine(p4, p1);
}