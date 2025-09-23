#pragma once
#include "../../gfx/ICanvas.h"
#include "../Shape.h"
#include "IShapeStrategy.h"
#include <array>
#include <sstream>
#include <iomanip>

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
            canvas->DrawTriangle(m_vertices[0], m_vertices[1], m_vertices[2]);
        }

        void Move(double dx, double dy) override
        {
            for (auto& vertex : m_vertices)
            {
                vertex.x += dx;
                vertex.y += dy;
            }
        }

        std::string GetInfo() const override
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1);
            oss << m_vertices[0].x << " " << m_vertices[0].y << " "
                << m_vertices[1].x << " " << m_vertices[1].y << " "
                << m_vertices[2].x << " " << m_vertices[2].y;
            return oss.str();
        }

        std::string GetType() const override
        {
            return "triangle";
        }

    private:
        std::array<Point, 3> m_vertices;
    };
}