#pragma once
#include "../../gfx/ICanvas.h"
#include "../Shape.h"
#include "IShapeStrategy.h"

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

    private:
        Point m_center;
        double m_radius;
    };
}