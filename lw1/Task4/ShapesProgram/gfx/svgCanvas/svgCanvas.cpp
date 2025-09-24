#include "svgCanvas.h"
#include <iostream>

svg::Color TransformColor(const gfx::Color& color)
{
    if (color.m_data.length() != 7 || color.m_data[0] != '#')
    {
        return svg::Color(0, 0, 0);
    }

    int r = std::stoi(color.m_data.substr(1, 2), nullptr, 16);
    int g = std::stoi(color.m_data.substr(3, 2), nullptr, 16);
    int b = std::stoi(color.m_data.substr(5, 2), nullptr, 16);

    return svg::Color(r, g, b);
}

gfx::CanvasSVG::CanvasSVG()
{
    std::cout << "Enter path to save: ";
    std::cin >> m_filePath;
    std::cout << "\n";

    m_document = svg::Document(m_filePath, svg::Layout(svg::Dimensions(1000, 1000), svg::Layout::TopLeft));
}

gfx::CanvasSVG::~CanvasSVG()
{
    m_document.save();
}

void gfx::CanvasSVG::MoveTo(double x, double y)
{
    m_x = x;
    m_y = y;
}

void gfx::CanvasSVG::SetColor(const Color& color)
{
    m_color = color;
}

void gfx::CanvasSVG::LineTo(double dx, double dy)
{
    svg::Color strokeColor = TransformColor(m_color);
    double endX = m_x + dx;
    double endY = m_y + dy;

    svg::Line line(
        svg::Point(m_x, m_y),
        svg::Point(endX, endY),
        svg::Stroke(m_LINE_WIDTH, strokeColor)
    );

    m_document << line;
    m_x = endX;
    m_y = endY;
    m_document.save();
}

void gfx::CanvasSVG::DrawRectangle(const shapes::Point& start, double width, double height)
{
    svg::Rectangle rect(
        svg::Point(start.x, start.y),
        width,
        height,
        svg::Fill(TransformColor(m_color))
    );

    m_document << rect;
    m_document.save();
}

void gfx::CanvasSVG::DrawTriangle(const shapes::Point& p1, const shapes::Point& p2, const shapes::Point& p3)
{
    svg::Color strokeColor = TransformColor(m_color);
    svg::Polygon triangle(
        svg::Fill(strokeColor),
        svg::Stroke(m_LINE_WIDTH, strokeColor)
    );
    triangle << svg::Point(p1.x, p1.y)
        << svg::Point(p2.x, p2.y)
        << svg::Point(p3.x, p3.y);

    m_document << triangle;
    m_document.save();
}

void gfx::CanvasSVG::DrawEllipse(double cx, double cy, double rx, double ry)
{
    svg::Elipse ellipse(
        svg::Point(cx, cy),
        rx * 2,
        ry * 2,
        svg::Fill(TransformColor(m_color))
    );

    m_document << ellipse;
    m_document.save();
}

void gfx::CanvasSVG::DrawText(double left, double top, double fontSize, const std::string& text)
{
    svg::Text textElem(
        svg::Point(left, top),
        text,
        svg::Fill(),
        svg::Font(fontSize, "Arial"),
        svg::Stroke(m_LINE_WIDTH, TransformColor(m_color))
    );

    m_document << textElem;
    m_document.save();
}