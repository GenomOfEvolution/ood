#pragma once
#include "ICommand.h"
#include "../Shape/Picture.h"

namespace UI
{
	class ChangeColorCommand : public ICommand
	{
	public:
		ChangeColorCommand(shapes::Picture& picture)
			: m_picture(picture) 
		{	};
		~ChangeColorCommand() = default;

		void Execute(std::istream& input) override
		{
			std::string id, colorStr;

			if (!(input >> id >> colorStr))
			{
				throw std::invalid_argument("Invalid ChangeColor command format");
			}

			auto shape = m_picture.GetShape(id);
			if (shape == nullptr)
			{
				throw std::invalid_argument("Shape with id '" + id + "' not found!");
			}
			
			gfx::Color color(colorStr);
			shape->SetColor(color);
		}
	private:
		shapes::Picture& m_picture;
	};
}; // namespace UI