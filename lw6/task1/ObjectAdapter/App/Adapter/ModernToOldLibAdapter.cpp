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
	CModernGraphicsRenderer::DrawLine(m_pos, { x, y });
}
