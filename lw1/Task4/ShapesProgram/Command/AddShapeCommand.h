#pragma once
#include "ICommand.h"
#include "../Shape/Picture.h"
#include "../Shape/ShapeStrategy/ShapeStrategyFactory.h"

namespace UI
{
	class AddShapeCommand : public ICommand
	{
	public:
		AddShapeCommand(shapes::Picture& picture)
			: m_picture(picture)
		{	};

		~AddShapeCommand() = default;

		void Execute(std::istream& input) override 
		{
			std::string id, colorStr, shapeType;

			if (!(input >> id >> colorStr >> shapeType)) 
			{
				throw std::invalid_argument("Invalid AddShape command format");
			}

			if (m_picture.GetShape(id) != nullptr)
			{
				throw std::invalid_argument("Shape with id '" + id + "' already exists");
			}

			gfx::Color color(colorStr);
			auto strategy = ShapeStrategyFactory::Create(shapeType, input);

			auto shape = std::make_unique<Shape>(std::move(strategy));
			shape->SetColor(color);

			m_picture.AddShape(id, std::move(shape));
		}

	private:
		shapes::Picture& m_picture;
	};
}; // namespace UI