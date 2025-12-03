#include "CommandHistory.h"

void CommandHistory::AddAndExecuteCommand(std::unique_ptr<ICommand>&& command)
{
	command->Execute();

	auto undoableCommand = dynamic_cast<IUndoableCommand*>(command.get());
	if (!undoableCommand)
	{
		return;
	}

	if (m_actionPos < m_commands.size())
	{
		m_commands.erase(m_commands.begin() + m_actionPos, m_commands.end());
	}

	m_commands.emplace_back(std::move(undoableCommand));
	m_actionPos++;
}

void CommandHistory::Clear()
{
	m_actionPos = 0;
	m_commands.clear();
}

bool CommandHistory::CanUndo() const
{
	return m_actionPos > 0;
}

void CommandHistory::Undo()
{
	if (CanUndo())
	{
		size_t prevIndex = --m_actionPos;
		m_commands[prevIndex]->Unexecute();
	}
}

bool CommandHistory::CanRedo() const
{
	return m_actionPos < m_commands.size();
}

void CommandHistory::Redo()
{
	if (CanRedo())
	{
		size_t nextIndex = m_actionPos++;
		m_commands[nextIndex]->Execute();
	}
}
