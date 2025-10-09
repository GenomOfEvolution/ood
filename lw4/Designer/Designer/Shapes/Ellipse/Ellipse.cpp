#include "Ellipse.h"

Ellipse::Ellipse(Color color, Point center, double horizontalRadius, double verticalRadius)
	: Shape(color)
	, m_center(center)
	, m_horizontalRadius(horizontalRadius)
	, m_verticalRadius(verticalRadius)
{
}

void Ellipse::Draw(ICanvas& canvas) const
{
	canvas.SetColor(GetColor());
	canvas.DrawEllipse(m_center, m_horizontalRadius, m_verticalRadius);
}
