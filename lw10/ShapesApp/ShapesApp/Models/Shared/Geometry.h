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

    bool ContainsPoint(const Point& point, double borderWidth = 15)
    {
        double leftBound = (x <= x + width) ? x : x + width;
        double rightBound = (x <= x + width) ? x + width : x;
        double topBound = (y <= y + height) ? y : y + height;
        double bottomBound = (y <= y + height) ? y + height : y;

        double halfBorder = borderWidth / 2.0;

        double outerLeft = leftBound - halfBorder;
        double outerRight = rightBound + halfBorder;
        double outerTop = topBound - halfBorder;
        double outerBottom = bottomBound + halfBorder;

        if (point.x < outerLeft || point.x > outerRight || point.y < outerTop || point.y > outerBottom) {
            return false;
        }

        double innerLeft = leftBound + halfBorder;
        double innerRight = rightBound - halfBorder;
        double innerTop = topBound + halfBorder;
        double innerBottom = bottomBound - halfBorder;

        bool innerRectValid = (innerLeft < innerRight) && (innerTop < innerBottom);

        if (innerRectValid &&
            point.x > innerLeft && point.x < innerRight &&
            point.y > innerTop && point.y < innerBottom) 
        {
            return false;
        }

        return true;
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