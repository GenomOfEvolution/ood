#include "CanvasSVG.h"
#include <iostream>

CanvasSvg::CanvasSvg()
{
    std::cout << "Enter path to save: ";
    std::cin >> m_filePath;
    std::cout << "\n";

    m_document = svg::Document(m_filePath, svg::Layout(svg::Dimensions(1000, 1000), svg::Layout::TopLeft));
}

CanvasSvg::~CanvasSvg()
{
    m_document.save();
}

void CanvasSvg::SetLineColor(std::optional<RGBAColor> color)
{
    m_outlineColor = color;
}

void CanvasSvg::SetFillColor(std::optional<RGBAColor> color)
{
    m_fillColor = color;
}

void CanvasSvg::SetLineThickness(std::optional<double> thickness)
{
    m_thickness = thickness;
}

void CanvasSvg::DrawPolygon(std::vector<PointD> points)
{
    svg::Polygon polygon(ColorToFill(m_fillColor), ColorToStroke(m_outlineColor));

    for (const auto& point : points) 
    {
        polygon << svg::Point(point.x, point.y);
    }

    m_document << polygon;
}

void CanvasSvg::DrawEllipse(const PointD& center, double horizontalRadius, double verticalRadius)
{
    svg::Elipse ellipse(
        { center.x, center.y },
        horizontalRadius * 2,
        verticalRadius * 2,
        ColorToFill(m_fillColor),
        ColorToStroke(m_outlineColor));

    m_document << ellipse;
}

svg::Color CanvasSvg::AdaptColor(const RGBAColor color) const
{
    uint8_t r = (color >> 24) & 0xFF; 
    uint8_t g = (color >> 16) & 0xFF;
    uint8_t b = (color >> 8) & 0xFF;
    uint8_t a = color & 0xFF;       

    if (a == 0) 
    {
        return svg::Color(svg::Color::Transparent);
    }
    
    return svg::Color(r, g, b);
}

svg::Fill CanvasSvg::ColorToFill(const std::optional<RGBAColor> color) const
{
    if (!color.has_value())
    {
        return svg::Fill(svg::Color::Transparent);
    }

    return svg::Fill(AdaptColor(*color));
}

svg::Stroke CanvasSvg::ColorToStroke(const std::optional<RGBAColor> color) const
{
    if (!color.has_value())
    {
        return svg::Stroke(svg::Color::Transparent);
    }

    if (m_thickness)
    {
        return svg::Stroke(*m_thickness, AdaptColor(*color));
    }
    else
    {
        return svg::Stroke(-1, AdaptColor(*color));
    }
}
