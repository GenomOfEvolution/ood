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

	private:
		modern_graphics_lib::CPoint m_pos{ 0, 0 };
	};
}
