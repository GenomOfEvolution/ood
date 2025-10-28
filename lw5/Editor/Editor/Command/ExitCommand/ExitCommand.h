#pragma once
#include "../AbstractCommand.h"
#include "../../Menu/Menu.h"

class ExitCommand : public AbstractCommand
{
public:
	ExitCommand(Menu& menu);

private:
	void DoExecute() override;

	Menu& m_menu;
};