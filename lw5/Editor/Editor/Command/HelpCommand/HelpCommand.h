#pragma once
#include "../AbstractCommand.h"
#include "../../Menu/Menu.h"

class HelpCommand : public AbstractCommand
{
public:
	HelpCommand(Menu& menu);

private:
	void DoExecute() override;

	Menu& m_menu;
};