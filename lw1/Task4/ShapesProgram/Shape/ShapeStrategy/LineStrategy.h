#pragma once
#include "../../gfx/ICanvas.h"
#include "../Shape.h"
#include "IShapeStrategy.h"
#include <array>
#include <sstream>
#include <iomanip>

namespace shapes
{
    class LineStrategy : public IShapeStrategy 
    {
    public:
        LineStrategy(Point startPoint, Point endPoint)
            : m_vertices{ startPoint, endPoint } 
        {
        }

        void Draw(gfx::ICanvas* canvas, gfx::Color color) const override 
        {
            canvas->SetColor(color);
            canvas->MoveTo(m_vertices[0].x, m_vertices[0].y);
            canvas->LineTo(m_vertices[1].x, m_vertices[1].y);
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
                << m_vertices[1].x << " " << m_vertices[1].y;
            return oss.str();
        }

        std::string GetType() const override
        {
            return "line";
        }

    private:
        std::array<Point, 2> m_vertices;
    };
}