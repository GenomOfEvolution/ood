#pragma once
#include "Color.h"
#include <string>

namespace shapes
{
    struct Point;
}

namespace gfx
{

    class ICanvas 
    {
    public:
        virtual ~ICanvas() = default;

        virtual void MoveTo(double x, double y) = 0;
        virtual void SetColor(const Color& color) = 0;
        virtual void LineTo(double dx, double dy) = 0;
        virtual void DrawRectangle(const shapes::Point& start, double width, double height) = 0;
        virtual void DrawTriangle(const shapes::Point& p1, const shapes::Point& p2, const shapes::Point& p3) = 0;
        virtual void DrawEllipse(double cx, double cy, double rx, double ry) = 0;
        virtual void DrawText(double left, double top, double fontSize, const std::string& text) = 0;
    };

} // namespace gfx