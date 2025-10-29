#pragma once
#include "../ICanvas.h"
#include "simple_svg_1.0.0.hpp"
#include <string>

class CanvasSvg : public ICanvas
{
public:
	CanvasSvg();
	CanvasSvg(const std::string& pathToFile)
		: m_filePath(pathToFile)
		, m_document(pathToFile, svg::Layout(svg::Dimensions(1000, 1000), svg::Layout::TopLeft))
	{	}
	~CanvasSvg();

	void SetLineColor(std::optional<RGBAColor> color) override;
	void SetFillColor(std::optional<RGBAColor> color) override;
	void SetLineThickness(std::optional<double> thickness) override;

	void DrawPolygon(std::vector<PointD> points) override;
	void DrawEllipse(const PointD& center, double horizontalRadius, double verticalRadius) override;

private:
	svg::Color AdaptColor(const RGBAColor color) const;
	svg::Fill ColorToFill(const std::optional<RGBAColor> color) const;
	svg::Stroke ColorToStroke(const std::optional<RGBAColor> color) const;

	std::optional<RGBAColor> m_fillColor;
	std::optional<RGBAColor> m_outlineColor;
	std::optional<double> m_thickness = 0;

	std::string m_filePath;
	svg::Document m_document;
};