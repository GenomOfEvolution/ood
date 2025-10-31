#include "MacroCommandBuilder.h"

MacroCommandBuilder::MacroCommandBuilder(std::istream& input, Menu& menu)
	: m_input(input)
	, m_menu(menu)
{
}

std::string MacroCommandBuilder::GetMacroName() const
{
	std::string macroName;
	while (true)
	{
		std::cout << "Enter macro name: ";
		std::getline(m_input, macroName);

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

std::string MacroCommandBuilder::GetMacroDescription() const
{
	std::string macroDescription;
	std::cout << "Enter macro description: ";
	std::getline(m_input, macroDescription);

	return macroDescription;
}

std::unique_ptr<MacroCommand> MacroCommandBuilder::BuildMacroCommand() const
{
	auto macroCommand = std::make_unique<MacroCommand>();
	std::cout << "Enter commands for the macro (enter " + m_END_MACRO + " to finish):" << std::endl;

	std::string input;
	while (true)
	{
		std::cout << "> ";
		std::getline(m_input, input);

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
