#pragma once
#include "ICanvasDrawable.h"

namespace shape_drawing_lib
{
    class CTriangle : public ICanvasDrawable
    {
    public:
        CTriangle(const Point& p1, const Point& p2, const Point& p3)
            : m_p1(p1) , m_p2(p2), m_p3(p3)
        {   }

        void Draw(graphics_lib::ICanvas& canvas) const override
        {
            canvas.MoveTo(m_p1.x, m_p1.y);

            canvas.LineTo(m_p2.x, m_p2.y);
            canvas.LineTo(m_p3.x, m_p3.y);
            canvas.LineTo(m_p1.x, m_p1.y);
        }

    private:
        Point m_p1, m_p2, m_p3;
    };
}// namespace

