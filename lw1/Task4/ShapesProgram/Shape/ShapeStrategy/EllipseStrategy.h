#pragma once
#include "../../gfx/ICanvas.h"
#include "../Shape.h"
#include "IShapeStrategy.h"
#include <sstream>
#include <iomanip>

namespace shapes
{
    class EllipseStrategy : public IShapeStrategy 
    {
    public:
        EllipseStrategy(Point center, double radius)
            : m_center(center), m_radius(radius) 
        {
        }

        void Draw(gfx::ICanvas* canvas, gfx::Color color) const override 
        {
            canvas->SetColor(color);
            canvas->DrawEllipse(m_center.x, m_center.y, m_radius, m_radius);
        }

        void Move(double dx, double dy) override 
        {
            m_center.x += dx;
            m_center.y += dy;
        }

        std::string GetInfo() const override
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1);
            oss << m_center.x << " " << m_center.y << " " << m_radius;
            return oss.str();
        }

        std::string GetType() const override
        {
            return "circle";
        }

    private:
        Point m_center;
        double m_radius;
    };
}