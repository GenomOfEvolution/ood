#include "svgCanvas.h"
#include <iostream>

svg::Color TransformColor(const Color& color)
{
    switch (color)
    {
    case Color::Green:
        return svg::Color(0, 128, 0); 
    case Color::Red:
        return svg::Color(255, 0, 0); 
    case Color::Blue:
        return svg::Color(0, 0, 255); 
    case Color::Yellow:
        return svg::Color(255, 255, 0); 
    case Color::Pink:
        return svg::Color(255, 192, 203); 
    case Color::Black:
        return svg::Color(0, 0, 0); 
    default:
        return svg::Color(0, 0, 0); 
    }
}

CanvasSVG::CanvasSVG()
{
    std::cout << "Enter path to save: ";
    std::cin >> m_filePath;
    std::cout << "\n";

    m_document = svg::Document(m_filePath, svg::Layout(svg::Dimensions(1000, 1000), svg::Layout::TopLeft));
}

void CanvasSVG::SetColor(Color color)
{
    m_color = color;
}

CanvasSVG::~CanvasSVG()
{
    m_document.save();
}

void CanvasSVG::DrawLine(const Point& from, const Point& to)
{
    svg::Color strokeColor = TransformColor(m_color);

    svg::Line line(
        svg::Point(from.x, from.y),
        svg::Point(to.x, to.y),
        svg::Stroke(m_LINE_WIDTH, strokeColor)
    );

    m_document << line;
    m_document.save();
}

void CanvasSVG::DrawEllipse(Point center, double w, double h)
{
    svg::Elipse ellipse(
        svg::Point(center.x, center.y), w, h, svg::Fill(TransformColor(m_color))
    );

    m_document << ellipse;
    m_document.save();
}