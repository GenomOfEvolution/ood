#pragma once
#include "ICommand.h"
#include "../Menu/Menu.h"

class MenuHelpCommand : public ICommand
{
public:
	MenuHelpCommand(const Menu& menu)
		:m_menu(menu)
	{	}

	void Execute() override
	{
		m_menu.ShowInstructions();
	}

	std::unique_ptr<ICommand> Clone() const override
	{
		return std::make_unique<MenuHelpCommand>(m_menu);
	}

private:
	const Menu& m_menu;
};
