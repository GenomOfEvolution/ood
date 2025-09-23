#pragma once
#include "ICommand.h"
#include "../Shape/Picture.h"
#include "../gfx/ICanvas.h"

namespace UI
{
	class DrawShapeCommand : public ICommand
	{
	public:
		DrawShapeCommand(const shapes::Picture& picture,
			gfx::ICanvas* canvas)
			: m_picture(picture)
			, m_canvas(canvas)
		{
			if (!m_canvas)
			{
				throw std::invalid_argument("Canvas cannot be null");
			}
		};

		~DrawShapeCommand() = default;

		void Execute(std::istream& input) override
		{
			std::string id;

			if (!(input >> id))
			{
				throw std::invalid_argument("Invalid DrawShapeCommand command format");
			}

			auto shape = m_picture.GetShape(id);
			if (shape == nullptr)
			{
				throw std::invalid_argument("Shape with id '" + id + "' not found!");
			}

			shape->Draw(m_canvas);
		}

	private:
		const shapes::Picture& m_picture;
		gfx::ICanvas* m_canvas;
	};
}; // namespace UI