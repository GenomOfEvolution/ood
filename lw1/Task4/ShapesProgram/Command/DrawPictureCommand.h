#pragma once
#include "ICommand.h"
#include "../Shape/Picture.h"
#include "../gfx/ICanvas.h"

namespace UI
{
	class DrawPictureCommand : public ICommand
	{
	public:
		DrawPictureCommand(const shapes::Picture& picture,
			gfx::ICanvas* canvas)
			: m_picture(picture)
			, m_canvas(canvas)
		{
			if (!m_canvas) 
			{
				throw std::invalid_argument("Canvas cannot be null");
			}
		};

		~DrawPictureCommand() = default;

		void Execute(std::istream& input) override
		{
			m_picture.DrawPicture(m_canvas);
		}

	private:
		const shapes::Picture& m_picture;
		gfx::ICanvas* m_canvas;
	};
}; // namespace UI