#pragma once
#include "../Document/IDocument.h"
#include "../Saver/ISaver.h"
#include "../History/IHistory.h"
#include "../Factory/ICommandFactory.h"

#include <map>
#include <memory>
#include <functional>
#include <string>

class Menu
{
public:
	Menu(std::shared_ptr<ISaver>&& saver,
		std::shared_ptr<IHistory>&& history,
		std::unique_ptr<IDocument>&& doc,
        std::unique_ptr<ICommandFactory>&& factory);

	void Run();
	void ShowInstructions() const;
	void Exit();

private:
	void ExecuteCommand();
	void HistoryExecute();

    const std::map<std::string, std::function<void()>> m_commandExecution =
    {
        { "Save", std::bind(&Menu::ExecuteCommand, this) },
        { "List", std::bind(&Menu::ExecuteCommand, this) },
        { "Undo", std::bind(&Menu::ExecuteCommand, this) },
        { "Redo", std::bind(&Menu::ExecuteCommand, this) },
        { "Help", std::bind(&Menu::ShowInstructions, this) },
        { "Exit", std::bind(&Menu::Exit, this) },

        { "InsertImage", std::bind(&Menu::HistoryExecute, this) },
        { "InsertParagraph", std::bind(&Menu::HistoryExecute, this) },
        { "SetTitle", std::bind(&Menu::HistoryExecute, this) },
        { "ReplaceText", std::bind(&Menu::HistoryExecute, this) },
        { "ResizeImage", std::bind(&Menu::HistoryExecute, this) },
        { "DeleteItem", std::bind(&Menu::HistoryExecute, this) },
    };

	std::shared_ptr<ISaver> m_saver;
	std::shared_ptr<IHistory> m_history;
	std::unique_ptr<IDocument> m_document;
    std::unique_ptr<ICommandFactory> m_commandFactory;

	bool m_exit = false;
    std::string m_currentInput;
};