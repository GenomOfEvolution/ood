#pragma once
#include "ICommand.h"
#include <iostream>
#include "CommandProcessor.h"

namespace UI
{
	class HelpCommand : public ICommand
	{
	public:
		HelpCommand(const CommandProcessor& menu)
			: m_menu(menu)
		{	};
		~HelpCommand() = default;

		void Execute(std::istream& input) override
		{
			m_menu.ShowInstructions();
		}
	private:
		const CommandProcessor& m_menu;
	};
}; // namespace UI