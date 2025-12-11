#include "CommandHistory.h"
#include "../Command/MegrableCommands/IMergableCommand.h"

void CommandHistory::AddAndExecuteCommand(std::unique_ptr<ICommand>&& command)
{
	command->Execute();

	if (auto undoable = dynamic_cast<IUndoableCommand*>(command.get())) 
	{
		if (m_actionPos < m_commands.size()) 
		{
			m_commands.erase(m_commands.begin() + m_actionPos, m_commands.end());
		}

		if (CanMergeWithLastCommand(command))
		{
			return;
		}

		m_commands.push_back(
			std::unique_ptr<IUndoableCommand>(static_cast<IUndoableCommand*>(command.release()))
		);
		m_actionPos++;
	}
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

bool CommandHistory::CanMergeWithLastCommand(std::unique_ptr<ICommand>& newCommand)
{
	if (m_commands.empty())
	{
		return false;
	}

	auto& lastCommand = m_commands.back();

	auto lastMergeable = dynamic_cast<IMergableCommand*>(lastCommand.get());
	auto newMergeable = dynamic_cast<IMergableCommand*>(newCommand.get());

	if (lastMergeable && newMergeable && lastMergeable->CanMergeWith(*newMergeable))
	{
		lastMergeable->MergeWith(std::move(newCommand));
		return true;
	}

	return false;
}