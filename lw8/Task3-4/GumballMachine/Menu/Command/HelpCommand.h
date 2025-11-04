#pragma once
#include "ICommand.h"
#include "../Menu.h"

class HelpCommand : public ICommand
{
public:
	HelpCommand(Menu& menu) 
		: m_menu(menu) 
	{};

	void Execute(std::istream& input) override
	{
		m_menu.ShowInstructions();
	};

private:
	Menu& m_menu;
};