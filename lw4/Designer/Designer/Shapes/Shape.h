#pragma once
#include "../Canvas/ICanvas.h"

class Shape
{
public:
	Shape(Color color) : m_color(color) {};

	Color GetColor() const { return m_color; };
	virtual void Draw(ICanvas& canvas) const = 0;

	virtual ~Shape() = default;

private:
	Color m_color;
};