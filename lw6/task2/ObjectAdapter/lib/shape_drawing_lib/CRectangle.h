#pragma once
#include "ICanvasDrawable.h"

namespace shape_drawing_lib
{
    class CRectangle : public ICanvasDrawable
    {
    public:
        CRectangle(const Point& leftTop, int width, int height, uint32_t color = 0x000000)
            : m_leftTop(leftTop), m_width(width), m_height(height), m_color(color)
        {   }

        void Draw(graphics_lib::ICanvas& canvas) const override
        {
            canvas.SetColor(m_color);
            canvas.MoveTo(m_leftTop.x, m_leftTop.y);

            canvas.LineTo(m_leftTop.x + m_width, m_leftTop.y);
            canvas.LineTo(m_leftTop.x + m_width, m_leftTop.y + m_height);
            canvas.LineTo(m_leftTop.x, m_leftTop.y + m_height);
            canvas.LineTo(m_leftTop.x, m_leftTop.y);
        }

    private:
        Point m_leftTop;
        int m_width, m_height;
        uint32_t m_color;
    };
}//namespace