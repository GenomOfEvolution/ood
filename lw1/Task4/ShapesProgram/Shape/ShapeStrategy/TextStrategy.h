#pragma once
#include "../../gfx/ICanvas.h"
#include "../Shape.h"
#include "IShapeStrategy.h"
#include <sstream>
#include <iomanip>

namespace shapes
{
    class TextStrategy : public IShapeStrategy
    {
    public:
        TextStrategy(Point leftTop, double size, std::string text)
            : m_leftTop(leftTop), m_size(size), m_text(std::move(text))
        {
        }

        void Draw(gfx::ICanvas* canvas, gfx::Color color) const override
        {
            canvas->SetColor(color);
            canvas->DrawText(m_leftTop.x, m_leftTop.y, m_size, m_text);
        }

        void Move(double dx, double dy) override
        {
            m_leftTop.x += dx;
            m_leftTop.y += dy;
        }

        std::string GetInfo() const override
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1);
            oss << m_leftTop.x << " " << m_leftTop.y << " " << m_size << " " << m_text;
            return oss.str();
        }

        std::string GetType() const override
        {
            return "text";
        }

    private:
        Point m_leftTop;
        double m_size;
        std::string m_text;
    };
}