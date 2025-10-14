#pragma once
// TODO: Canvas не должкен зависеть от фигур
// ÷вет перенести в Canvas
#include "../Shapes/ShapeTypes.h"

enum Color
{
	Green,
	Red,
	Blue,
	Yellow,
	Pink,
	Black,
};

class ICanvas
{
public:
	virtual void SetColor(Color color) = 0;
	virtual void DrawLine(const Point& from, const Point& to) = 0;
	virtual void DrawEllipse(Point center, double w, double h) = 0;

	// TODO: повторить зачем нужен виртуальный деструктор, сказать когда примение не виртуального деструкора может привести к проблемам
	virtual ~ICanvas() = default;
};
