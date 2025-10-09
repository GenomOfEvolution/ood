#pragma once
#include "simple_svg_1.0.0.hpp"
#include "../ICanvas.h"
#include "../../Shapes/Shape.h"
#include "../../Shapes/ShapeTypes.h"

#include <string>


class CanvasSVG : public ICanvas
{
public:
	CanvasSVG();
	CanvasSVG(const std::string& pathToFile)
		: m_filePath(pathToFile)
		, m_document(pathToFile, svg::Layout(svg::Dimensions(1000, 1000), svg::Layout::TopLeft))
	{
	}
	~CanvasSVG();

	void SetColor(Color color) override;
	void DrawLine(const Point& from, const Point& to)  override;
	void DrawEllipse(Point center, double w, double h)  override;

private:
	Color m_color = Color::Black;
	std::string m_filePath;
	svg::Document m_document;

	const int m_LINE_WIDTH = 3;
};
