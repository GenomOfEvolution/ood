#pragma once
#include "../lib/shape_drawing_lib/CCanvasPainter.h"
#include "../lib/modern_graphics_lib/CModernGraphicsRenderer.h"

namespace app
{
	void PaintPicture(shape_drawing_lib::CCanvasPainter& painter);
	void PaintPictureOnCanvas();
	void PaintPictureOnModernGraphicsRenderer();
}// namespace