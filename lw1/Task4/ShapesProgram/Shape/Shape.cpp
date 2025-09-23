#include "Shape.h"

void shapes::Shape::Move(double dx, double dy)
{
	m_shapeStrategy->Move(dx, dy);
}

void shapes::Shape::Draw(gfx::ICanvas* canvas) const
{
	m_shapeStrategy->Draw(canvas, m_color);
}

std::string shapes::Shape::GetInfo() const
{
	return m_shapeStrategy->GetInfo();
}

std::string shapes::Shape::GetType() const
{
	return m_shapeStrategy->GetType();
}

gfx::Color shapes::Shape::GetColor() const
{
	return m_color;
}

void shapes::Shape::SetColor(gfx::Color newColor)
{
	m_color = newColor;
}

void shapes::Shape::SetStrategy(std::unique_ptr<IShapeStrategy>&& newStrategy)
{
	m_shapeStrategy = std::move(newStrategy);
}

