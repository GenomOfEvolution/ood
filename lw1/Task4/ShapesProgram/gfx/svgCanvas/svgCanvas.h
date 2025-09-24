#pragma once
#include <string>

#include "simple_svg_1.0.0.hpp"
#include "../ICanvas.h"
#include "../Color.h"
#include "../../Shape/Shape.h"

namespace gfx
{
	class CanvasSVG : public ICanvas
	{
	public:
		CanvasSVG();
		CanvasSVG(const std::string& pathToFile)
			: m_filePath(pathToFile)
			, m_document(pathToFile, svg::Layout(svg::Dimensions(1000, 1000), svg::Layout::TopLeft))
		{	}
		~CanvasSVG();

		void MoveTo(double x, double y) override;
		void SetColor(const Color& color) override;
		void LineTo(double dx, double dy) override;
		void DrawRectangle(const shapes::Point& start, double width, double height) override;
		void DrawTriangle(const shapes::Point& p1, const shapes::Point& p2, const shapes::Point& p3) override;
		void DrawEllipse(double cx, double cy, double rx, double ry) override;
		void DrawText(double left, double top, double fontSize, const std::string& text) override;

	private:
		gfx::Color m_color;
		std::string m_filePath;
		double m_x = 0;
		double m_y = 0;
		svg::Document m_document;

		const int m_LINE_WIDTH = 3;
	};
}