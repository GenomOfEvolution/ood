#pragma once
#include "ICommand.h"
#include "../Shape/Picture.h"

namespace UI
{
	class MovePictureCommand : public ICommand
	{
	public:
		MovePictureCommand(shapes::Picture& picture)
			: m_picture(picture)
		{
		};
		~MovePictureCommand() = default;

		void Execute(std::istream& input) override
		{
			double dx, dy;

			if (!(input >> dx >> dy))
			{
				throw std::invalid_argument("Invalid ChangeColor command format");
			}
			
			m_picture.MovePicture(dx, dy);
		}
	private:
		shapes::Picture& m_picture;
	};
}; // namespace UI