#pragma once
#include "../Shape/Shape.h"

#include "ICanvas.h"
#include <iostream>

namespace gfx {
    class Canvas : public ICanvas {
    public:
        void MoveTo(double x, double y) override {
            std::cout << "MoveTo(" << x << ", " << y << ")" << std::endl;
        }

        void SetColor(Color color) override {
            std::cout << "SetColor(" << color.m_data << ")" << std::endl;
        }

        void LineTo(double dx, double dy) override {
            std::cout << "LineTo(" << dx << ", " << dy << ")" << std::endl;
        }

        void DrawRectangle(shapes::Point start, double width, double height) override {
            std::cout << "DrawRectangle((" << start.x << ", " << start.y
                << "), " << width << ", " << height << ")" << std::endl;
        }

        void DrawTriangle(shapes::Point p1, shapes::Point p2, shapes::Point p3) override {
            std::cout << "DrawTriangle((" << p1.x << ", " << p1.y
                << "), (" << p2.x << ", " << p2.y
                << "), (" << p3.x << ", " << p3.y << "))" << std::endl;
        }

        void DrawEllipse(double cx, double cy, double rx, double ry) override {
            std::cout << "DrawEllipse(" << cx << ", " << cy
                << ", " << rx << ", " << ry << ")" << std::endl;
        }

        void DrawText(double left, double top, double fontSize, const std::string& text) override {
            std::cout << "DrawText(" << left << ", " << top
                << ", " << fontSize << ", " << text << ")" << std::endl;
        }
    };
} // namespace gfx