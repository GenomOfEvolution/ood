#pragma once
#include <algorithm>

struct Point
{
    double x = 0.0;
    double y = 0.0;

    Point() = default;
    Point(double x, double y) : x(x), y(y) {}

    Point operator+(const Point& other) const 
    {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) const 
    {
        return Point(x - other.x, y - other.y);
    }

    Point& operator+=(const Point& other) 
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point& operator-=(const Point& other) 
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    bool operator==(const Point& other) const 
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const 
    {
        return !(*this == other);
    }
};

struct Rect
{
    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;

    Rect() = default;
    Rect(double x, double y, double w, double h)
        : x(x), y(y), width(w), height(h)
    {
    }

    Rect(const Point& leftTop, const Point& rightBottom)
        : x(leftTop.x), y(leftTop.y),
        width(rightBottom.x - leftTop.x), height(rightBottom.y - leftTop.y)
    {
    }

    Point GetLeftTop() const 
    {
        return Point(x, y);
    }

    Point GetRightBottom() const 
    {
        return Point(x + width, y + height);
    }

    double GetWidth() const 
    {
        return width;
    }

    double GetHeight() const 
    {
        return height;
    }
};