#include "ExitCommand.h"

ExitCommand::ExitCommand(Menu& menu)
	: m_menu(menu)
{
}

void ExitCommand::DoExecute()
{
	m_menu.Exit();
}
