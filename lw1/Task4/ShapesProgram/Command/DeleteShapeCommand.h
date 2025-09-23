#pragma once
#include "ICommand.h"
#include "../Shape/Picture.h"

namespace UI
{
	class DeleteShapeCommand : public ICommand
	{
	public:
		DeleteShapeCommand(shapes::Picture& picture)
			: m_picture(picture)
		{
		};
		~DeleteShapeCommand() = default;

		void Execute(std::istream& input) override
		{
			std::string id;

			if (!(input >> id))
			{
				throw std::invalid_argument("Invalid DeleteShape command format");
			}

			if (m_picture.GetShape(id) == nullptr)
			{
				throw std::invalid_argument("Shape with id '" + id + "' not found!");
			}

			m_picture.DeleteShape(id);
		}
	private:
		shapes::Picture& m_picture;
	};
}; // namespace UI