#pragma once
#include "../Shapes/ShapeTypes.h"

class ICanvas
{
public:
	virtual void SetColor(Color color) = 0;
	virtual void DrawLine(const Point& from, const Point& to) = 0;
	virtual void DrawEllipse(Point center, double w, double h) = 0;

	virtual ~ICanvas() = default;
};
