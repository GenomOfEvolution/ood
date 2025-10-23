#include "ModernToOldLibAdapter.h"

app::ModernToOldLibAdapter::ModernToOldLibAdapter(std::ostream& out)
	: CModernGraphicsRenderer::CModernGraphicsRenderer(out)
{
	CModernGraphicsRenderer::BeginDraw();
}

app::ModernToOldLibAdapter::~ModernToOldLibAdapter()
{
	CModernGraphicsRenderer::EndDraw();
}

void app::ModernToOldLibAdapter::MoveTo(int x, int y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void app::ModernToOldLibAdapter::LineTo(int x, int y)
{
	CModernGraphicsRenderer::DrawLine(m_pos, { x, y }, m_color);
}

void app::ModernToOldLibAdapter::SetColor(uint32_t rgbColor)
{
	m_color = FromRGB(rgbColor);
}

modern_graphics_lib::CRGBAColor app::ModernToOldLibAdapter::FromRGB(uint32_t rgb)
{
	return modern_graphics_lib::CRGBAColor
	(
		((rgb >> 16) & 0xFF) / 255.0f,  
		((rgb >> 8) & 0xFF) / 255.0f,  
		(rgb & 0xFF) / 255.0f,         
		1.0f                            
	);
}
