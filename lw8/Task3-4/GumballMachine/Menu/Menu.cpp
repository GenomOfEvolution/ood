#include "Menu.h"
#include <iostream>

void Menu::AddItem(
	const std::string& shortcut,
	const std::string& description,
	std::unique_ptr<ICommand>&& command)
{
	m_items.emplace_back(shortcut, description, std::move(command));
}

void Menu::Run()
{
	ShowInstructions();

	std::string command;
	while ((std::cout << ">")
		&& getline(std::cin, command)
		&& ExecuteCommand(command))
	{
	}
}

void Menu::ShowInstructions() const
{
	std::cout << "Commands list:\n";
	for (auto& item : m_items)
	{
		std::cout << "  " << item.shortcut << ": " << item.description << "\n";
	}
}

void Menu::Exit()
{
	m_exit = true;
}

bool Menu::ExecuteCommand(const std::string& command)
{
	m_exit = false;

	auto it = std::find_if(m_items.begin(), m_items.end(), [&](const Item& item) {
		return item.shortcut == command;
	});

	if (it != m_items.end())
	{
		it->command->Execute(std::cin);
	}
	else
	{
		if (command.empty())
		{
			std::cout << std::endl;
		}
		else
		{
			std::cout << "Unknown command: " + command + "\n";
		}
	}

	return !m_exit;
}