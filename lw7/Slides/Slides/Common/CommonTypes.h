#pragma once
#include <iostream>

template <typename T>
struct Rect
{
	T left;
	T top;
	T width;
	T height;

	friend bool operator==(const Rect& lhs, const Rect& rhs) 
	{
		return lhs.left == rhs.left
			&& lhs.top == rhs.top
			&& lhs.width == rhs.width
			&& lhs.height == rhs.height;
	}
};

template <typename T>
struct Point
{
	T x;
	T y;
};

using RectD = Rect<double>;
using PointD = Point<double>;
using RGBAColor = uint32_t;