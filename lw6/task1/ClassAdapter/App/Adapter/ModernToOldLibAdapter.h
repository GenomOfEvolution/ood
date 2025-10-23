#pragma once
#include "../../lib/graphics_lib/ICanvas.h"
#include "../../lib/modern_graphics_lib/CModernGraphicsRenderer.h"
#include <iostream>

namespace app
{
	class ModernToOldLibAdapter
		: public graphics_lib::ICanvas
		, private modern_graphics_lib::CModernGraphicsRenderer
	{
	public:
		ModernToOldLibAdapter(std::ostream& out);
		~ModernToOldLibAdapter() override;

		void MoveTo(int x, int y) override;
		void LineTo(int x, int y) override;
		void SetColor(uint32_t rgbColor) override;

	private:
		modern_graphics_lib::CPoint m_pos{ 0, 0 };
		modern_graphics_lib::CRGBAColor m_color { 0.0f, 0.0f, 0.0f, 1.0f };

		modern_graphics_lib::CRGBAColor FromRGB(uint32_t rgb);
	};
}
