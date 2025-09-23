#pragma once
#include "ICommand.h"
#include <iostream>
#include "CommandProcessor.h"

namespace UI
{
	class ExitCommand : public ICommand
	{
	public:
		ExitCommand(CommandProcessor& menu)
			: m_menu(menu)
		{	};
		~ExitCommand() = default;

		void Execute(std::istream& input) override
		{
			m_menu.Exit();
		}
	private:
		CommandProcessor& m_menu;
	};
}; // namespace UI