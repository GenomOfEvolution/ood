#pragma once
#include "../../gfx/ICanvas.h"
#include "../Shape.h"
#include "IShapeStrategy.h"

namespace shapes
{
    class RectangleStrategy : public IShapeStrategy 
    {
    public:
        RectangleStrategy(Point leftTop, double width, double height)
            : m_leftTop(leftTop), m_width(width), m_height(height) 
        {
        }

        void Draw(gfx::ICanvas* canvas, gfx::Color color) const override 
        {
            canvas->SetColor(color);
            canvas->DrawRectangle(m_leftTop, m_width, m_height);
        }

        void Move(double dx, double dy) override 
        {
            m_leftTop.x += dx;
            m_leftTop.y += dy;
        }

    private:
        Point m_leftTop;
        double m_width;
        double m_height;
    };
}