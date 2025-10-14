#include "MakeMacroCommand.h"
#include <string>
#include <iostream>

MakeMacroCommand::MakeMacroCommand(Menu& menu)
	: m_menu(menu)
{
}

void MakeMacroCommand::Execute()
{
	std::string macroName = GetMacroName();
	std::string macroDescription = GetMacroDescription();
	auto macroCommand = BuildMacroCommand();

	m_menu.AddItem(macroName, macroDescription, std::move(macroCommand));
}

std::unique_ptr<ICommand> MakeMacroCommand::Clone() const
{
	return std::make_unique<MakeMacroCommand>(m_menu);
}

std::string MakeMacroCommand::GetMacroName() const
{
	std::string macroName;
	while (true)
	{
		std::cout << "Enter macro name: ";
		std::getline(std::cin, macroName);

		if (macroName.empty())
		{
			continue;
		}

		if (m_menu.HasCommand(macroName))
		{
			std::cout << "This name is already taken!" << std::endl;
		}
		else
		{
			break;
		}
	}

	return macroName;
}

std::string MakeMacroCommand::GetMacroDescription() const
{
	std::string macroDescription;
	std::cout << "Enter macro description: ";
	std::getline(std::cin, macroDescription);

	return macroDescription;
}

std::unique_ptr<MacroCommand> MakeMacroCommand::BuildMacroCommand() const
{
	auto macroCommand = std::make_unique<MacroCommand>();
	std::cout << "Enter commands for the macro (enter " + m_END_MACRO + " to finish):" << std::endl;

	std::string input;
	while (true)
	{
		std::cout << "> ";
		std::getline(std::cin, input);

		if (input == m_END_MACRO)
		{
			break;
		}

		if (input.empty())
		{
			continue;
		}

		auto clonedCommand = m_menu.CloneCommand(input);
		if (clonedCommand)
		{
			macroCommand->AddCommand(std::move(clonedCommand));
		}
		else
		{
			std::cout << "Unknown or forbidden command: " << input << std::endl;
		}
	}

	return macroCommand;
}
