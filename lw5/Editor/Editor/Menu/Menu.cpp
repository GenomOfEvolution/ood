#include "Menu.h"
#include "../Factory/CommandFactory.h"
#include <iostream>
#include <sstream>

Menu::Menu(
	std::shared_ptr<ISaver>&& saver,
	std::shared_ptr<ICommandExecutor>&& history,
	std::unique_ptr<IDocument>&& doc)
	: m_document(std::move(doc))
	, m_saver(std::move(saver))
	, m_history(std::move(history))
	, m_commandFactory(std::make_unique<CommandFactory>(*m_document, *m_saver, *this))
{
}

void Menu::ShowInstructions() const
{
	std::cout << "Available commands:\n"
		<< "InsertParagraph <position>|end <text>\n"
		<< "InsertImage <position>|end <width> <height> <image path>\n"
		<< "SetTitle <document title>\n"
		<< "List\n"
		<< "ReplaceText <position> <paragraph text>\n"
		<< "ResizeImage <position> <width> <height>\n"
		<< "DeleteItem <position>\n"
		<< "Help\n"
		<< "Undo\n"
		<< "Redo\n"
		<< "Save <path>\n"
		<< std::endl;
}

void Menu::Run()
{
	ShowInstructions();

	std::string command;
	while (!m_exit 
		&& (std::cout << ">")
		&& getline(std::cin, command))
	{
		try
		{
			auto cmd = m_commandFactory->CreateCommand(command);
			m_history->AddAndExecuteCommand(std::move(cmd));
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void Menu::Exit()
{
	m_exit = true;
}
