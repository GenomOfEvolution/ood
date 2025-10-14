#pragma once
#include "ICommand.h"
#include "../Menu/Menu.h"

class ExitMenuCommand : public ICommand
{
public:
	ExitMenuCommand(Menu& menu)
		: m_menu(menu)
	{	}

	void Execute() override
	{
		m_menu.Exit();
	}

	std::unique_ptr<ICommand> Clone() const override
	{
		return std::make_unique<ExitMenuCommand>(m_menu);
	}

private:
	Menu& m_menu;
};