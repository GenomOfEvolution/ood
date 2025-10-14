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

std::unique_ptr<ICommand> Menu::CloneCommand(const std::string& shortcut) const
{
	for (const auto& item : m_items) 
	{
		if (item.shortcut == shortcut && shortcut != "begin_macro") 
		{
			return item.command->Clone();
		}
	}

	return nullptr;
}

bool Menu::HasCommand(const std::string& shortcut) const
{
	for (const auto& item : m_items) 
	{
		if (item.shortcut == shortcut) 
		{
			return true;
		}
	}
	return false;
}

bool Menu::ExecuteCommand(const std::string& command)
{
	m_exit = false;

	auto it = std::find_if(m_items.begin(), m_items.end(), [&](const Item& item) {
		return item.shortcut == command;
	});

	if (it != m_items.end())
	{
		it->command->Execute();
	}
	else
	{
		std::cout << "Unknown command\n";
	}

	return !m_exit;
}