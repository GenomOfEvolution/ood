#include "MakeMacroCommand.h"
#include "MacroCommandBuilder/MacroCommandBuilder.h"
#include <string>
#include <iostream>

MakeMacroCommand::MakeMacroCommand(Menu& menu)
	: m_menu(menu)
	, m_builder(std::move(std::make_unique<MacroCommandBuilder>(std::cin, m_menu)))
{
}

void MakeMacroCommand::Execute()
{
	std::string macroName = m_builder->GetMacroName();
	std::string macroDescription = m_builder->GetMacroDescription();
	auto macroCommand = m_builder->BuildMacroCommand();

	m_menu.AddItem(macroName, macroDescription, std::move(macroCommand));
}

std::unique_ptr<ICommand> MakeMacroCommand::Clone() const
{
	return std::make_unique<MakeMacroCommand>(m_menu);
}