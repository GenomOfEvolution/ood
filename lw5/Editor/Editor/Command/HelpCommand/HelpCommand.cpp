#include "HelpCommand.h"

HelpCommand::HelpCommand(Menu& menu)
	: m_menu(menu)
{
}

void HelpCommand::DoExecute()
{
	m_menu.ShowInstructions();
}
