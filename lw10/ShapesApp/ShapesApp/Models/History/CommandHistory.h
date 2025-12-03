#pragma once
#include "ICommandExecutor.h"
#include "IHistory.h"
#include "../Command/IUndoableCommand.h"

#include <vector>

class CommandHistory : public IHistory, public ICommandExecutor
{
public:
	void AddAndExecuteCommand(std::unique_ptr<ICommand>&& command) override;
	void Clear() override;

	bool CanUndo() const override;
	void Undo() override;

	bool CanRedo() const override;
	void Redo() override;

private:
	size_t m_actionPos = 0;
	std::vector<std::unique_ptr<IUndoableCommand>> m_commands;
};