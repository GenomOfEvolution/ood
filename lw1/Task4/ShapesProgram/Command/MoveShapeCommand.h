#pragma once
#include "ICommand.h"
#include "../Shape/Picture.h"

namespace UI
{
	class MoveShapeCommand : public ICommand
	{
	public:
		MoveShapeCommand(shapes::Picture& picture)
			: m_picture(picture)
		{
		};
		~MoveShapeCommand() = default;

		void Execute(std::istream& input) override
		{
			std::string id;
			double dx, dy;

			if (!(input >> id >> dx >> dy))
			{
				throw std::invalid_argument("Invalid ChangeColor command format");
			}

			auto shape = m_picture.GetShape(id);
			if (shape == nullptr)
			{
				throw std::invalid_argument("Shape with id '" + id + "' not found!");
			}

			shape->Move(dx, dy);
		}
	private:
		shapes::Picture& m_picture;
	};
}; // namespace UI