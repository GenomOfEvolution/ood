#include "CommandProcessor.h"
#include <ranges>
#include <sstream>
#include <exception>

void UI::CommandProcessor::AddItem(const std::string& shortcut, const std::string& description, std::unique_ptr<ICommand>&& command)
{
    m_items.emplace_back(shortcut, description, move(command));
}

void UI::CommandProcessor::Run()
{
    std::string command;
    while ((m_output << ">")
        && std::getline(m_input, command)
        && ExecuteCommand(command))
    {
    }
}

void UI::CommandProcessor::ShowInstructions() const
{
    m_output << "Commands list:\n";
    for (auto& item : m_items)
    {
        m_output << "  " << item.shortcut << ": " << item.description << "\n";
    }
}

void UI::CommandProcessor::Exit()
{
	m_exit = true;
}

bool UI::CommandProcessor::ExecuteCommand(const std::string& commandLine)
{
    m_exit = false;

    std::istringstream iss(commandLine);
    std::string commandName;
    iss >> commandName;

    auto it = std::ranges::find_if(m_items, [&](const Item& item)
    {
        return item.shortcut == commandName;
    });

    if (it != m_items.end())
    {
        try 
        {
            it->command->Execute(iss);
        }
        catch (const std::exception& e) 
        {
            m_output << "Error: " << e.what() << "\n";
        }
    }
    else
    {
        std::cout << "Unknown command\n";
    }

    return !m_exit;
}