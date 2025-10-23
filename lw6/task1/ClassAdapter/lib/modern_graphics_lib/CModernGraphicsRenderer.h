#pragma once
#include <iostream>
#include <format>

namespace modern_graphics_lib
{
    class CPoint
    {
    public:
        CPoint(int x, int y) :x(x), y(y) {}
        int x;
        int y;
    };

    class CModernGraphicsRenderer
    {
    public:
        CModernGraphicsRenderer(std::ostream& strm) 
            : m_out(strm)
        {   }

        ~CModernGraphicsRenderer()
        {
            if (m_drawing)
            {
                EndDraw();
            }
        }

        void BeginDraw()
        {
            if (m_drawing)
            {
                throw std::logic_error("Drawing has already begun");
            }
            m_out << "<draw>" << std::endl;
            m_drawing = true;
        }

        void DrawLine(const CPoint& start, const CPoint& end)
        {
            if (!m_drawing)
            {
                throw std::logic_error("DrawLine is allowed between BeginDraw()/EndDraw() only");
            }

            m_out << std::format(R"(  <line fromX="{}" fromY="{}" toX="{}" toY="{}"/>)",
                start.x, start.y, end.x, end.y) << std::endl;
        }

        void EndDraw()
        {
            if (!m_drawing)
            {
                throw std::logic_error("Drawing has not been started");
            }
            m_out << "</draw>" << std::endl;
            m_drawing = false;
        }

    private:
        std::ostream& m_out;
        bool m_drawing = false;
    };
}// namespace