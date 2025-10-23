#pragma once
#include "../../lib/graphics_lib/ICanvas.h"
#include "../../lib/modern_graphics_lib/CModernGraphicsRenderer.h"
#include <iostream>

namespace app
{
	class ModernToOldLibAdapter	: public graphics_lib::ICanvas
	{
	public:
		ModernToOldLibAdapter(modern_graphics_lib::CModernGraphicsRenderer& renderer);
		~ModernToOldLibAdapter() override;

		void MoveTo(int x, int y) override;
		void LineTo(int x, int y) override;

	private:
		modern_graphics_lib::CModernGraphicsRenderer& m_renderer;
		modern_graphics_lib::CPoint m_pos{ 0, 0 };
	};
}
