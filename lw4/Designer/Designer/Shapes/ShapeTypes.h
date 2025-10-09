#pragma once

enum Color
{
	Green,
	Red,
	Blue,
	Yellow,
	Pink,
	Black,
};

struct Point
{
	double x = 0, y = 0;
	bool operator==(const Point& other) const 
	{
		return ((other.x == x) && (other.y == y));
	}
};