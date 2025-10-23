#include "ModernToOldLibAdapter.h"

app::ModernToOldLibAdapter::ModernToOldLibAdapter(modern_graphics_lib::CModernGraphicsRenderer& renderer)
	: m_renderer(renderer)
{
	m_renderer.BeginDraw();
}

app::ModernToOldLibAdapter::~ModernToOldLibAdapter()
{
	m_renderer.EndDraw();
}

void app::ModernToOldLibAdapter::MoveTo(int x, int y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void app::ModernToOldLibAdapter::LineTo(int x, int y)
{
	m_renderer.DrawLine(m_pos, { x, y });
}
