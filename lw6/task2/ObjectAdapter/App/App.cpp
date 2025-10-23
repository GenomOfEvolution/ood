#include "App.h"
#include "../lib/shape_drawing_lib/CRectangle.h"
#include "../lib/shape_drawing_lib/CTriangle.h"
#include "../lib/graphics_lib/CCanvas.h"
#include "../lib/modern_graphics_lib/CModernGraphicsRenderer.h"
#include "Adapter/ModernToOldLibAdapter.h"
#include <iostream>

void app::PaintPicture(shape_drawing_lib::CCanvasPainter& painter)
{
	using namespace shape_drawing_lib;
	CRectangle rect({ 200, 200 }, 400, 400, 0xff00ff);
	CTriangle triangle({ 200, 200 }, { 300, 100 }, { 200, 400 }, 0x00ff00);

	painter.Draw(rect);
	painter.Draw(triangle);
}

void app::PaintPictureOnCanvas()
{
	using namespace shape_drawing_lib;
	using namespace graphics_lib;

	CCanvas canvas;
	CCanvasPainter painter(canvas);
	PaintPicture(painter);
}

void app::PaintPictureOnModernGraphicsRenderer()
{
	using namespace shape_drawing_lib;
	using namespace graphics_lib;
	using namespace modern_graphics_lib;

	CModernGraphicsRenderer rendrer(std::cout);
	ModernToOldLibAdapter adapter(rendrer);
	CCanvasPainter painter(adapter);

	PaintPicture(painter);
}
