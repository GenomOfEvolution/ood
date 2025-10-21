#include "History.h"
#include "../Command/MergableCommand/IMergableCommand.h"

void History::AddAndExecuteCommand(std::unique_ptr<ICommand>&& command)
{
	command->Execute();

	if (m_actionPos < m_commands.size())
	{
		m_commands.erase(m_commands.begin() + m_actionPos, m_commands.end());
	}

	if (CanMergeWithLastCommand(command))
	{
		return;
	}

	m_commands.emplace_back(std::move(command));
	m_actionPos++;

	if (m_commands.size() > MAX_HISTORY_SIZE)
	{
		m_commands.erase(m_commands.begin());
		m_actionPos--;
	}
}

bool History::CanUndo() const
{
	return m_actionPos > 0;
}

void History::Undo()
{
	if (CanUndo())
	{
		size_t prevIndex = --m_actionPos;
		m_commands[prevIndex]->Unexecute();
	}
}

bool History::CanRedo() const
{
	return m_actionPos < m_commands.size();
}

void History::Redo()
{
	if (CanRedo())
	{
		size_t nextIndex = m_actionPos++;
		m_commands[nextIndex]->Unexecute();
	}
}

bool History::CanMergeWithLastCommand(std::unique_ptr<ICommand>& newCommand)
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