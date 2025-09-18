#pragma once
#include "../../gfx/ICanvas.h"
#include "../Shape.h"
#include "IShapeStrategy.h"
#include <array>

namespace shapes
{
    class TriangleStrategy : public IShapeStrategy 
    {
    public:
        TriangleStrategy(Point p1, Point p2, Point p3)
            : m_vertices{ p1, p2, p3 } 
        {
        }

        void Draw(gfx::ICanvas* canvas, gfx::Color color) const override 
        {
            canvas->SetColor(color);
            canvas->DrawTriangle(m_vertices[1], m_vertices[2], m_vertices[3]);
        }

        void Move(double dx, double dy) override 
        {
            for (auto& vertex : m_vertices) 
            {
                vertex.x += dx;
                vertex.y += dy;
            }
        }

    private:
        std::array<Point, 3> m_vertices;
    };
}