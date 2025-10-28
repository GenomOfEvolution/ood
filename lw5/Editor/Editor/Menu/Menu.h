#pragma once
#include "../Document/IDocument.h"
#include "../Saver/ISaver.h"
#include "../History/ICommandExecutor.h"
#include "../Factory/ICommandFactory.h"

#include <memory>
#include <string>

class Menu
{
public:
	Menu(std::shared_ptr<ISaver>&& saver,
		std::shared_ptr<ICommandExecutor>&& history,
		std::unique_ptr<IDocument>&& doc);

	void Run();
	void ShowInstructions() const;
	void Exit();

private:
	std::shared_ptr<ISaver> m_saver;
	std::shared_ptr<ICommandExecutor> m_history;
	std::unique_ptr<IDocument> m_document;
    std::unique_ptr<ICommandFactory> m_commandFactory;

	bool m_exit = false;
};