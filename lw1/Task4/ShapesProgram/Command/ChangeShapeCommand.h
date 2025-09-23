#pragma once
#include "ICommand.h"
#include "../Shape/Picture.h"
#include "../Shape/ShapeStrategy/ShapeStrategyFactory.h"

namespace UI
{
	class ChangeShapeCommand : public ICommand
	{
	public:
		ChangeShapeCommand(shapes::Picture& picture)
			: m_picture(picture)
		{
		};
		~ChangeShapeCommand() = default;

		void Execute(std::istream& input) override
		{
			std::string id, shapeType;

			if (!(input >> id >> shapeType))
			{
				throw std::invalid_argument("Invalid ChangeColor command format");
			}

			auto shape = m_picture.GetShape(id);
			if (shape == nullptr)
			{
				throw std::invalid_argument("Shape with id '" + id + "' not found!");
			}

			auto strategy = ShapeStrategyFactory::Create(shapeType, input);
			shape->SetStrategy(std::move(strategy));
		}
	private:
		shapes::Picture& m_picture;
	};
}; // namespace UI