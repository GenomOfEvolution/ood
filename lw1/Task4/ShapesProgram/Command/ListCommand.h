#pragma once
#include "ICommand.h"
#include <iostream>
#include "../Shape/Picture.h"

namespace UI
{
	class ListCommand : public ICommand
	{
	public:
		ListCommand(const shapes::Picture& picture)
			: m_picture(picture)
		{
		};
		~ListCommand() = default;

		void Execute(std::istream& input) override
		{
			std::cout << m_picture.GetAllInfo();
		}
	private:
		const shapes::Picture& m_picture;
	};
}; // namespace UI