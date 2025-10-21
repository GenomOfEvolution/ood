#pragma once
#include "IHistory.h"
#include "../Command/ICommand.h"
#include <vector>
#include <memory>

class History : public IHistory
{
public:
	void AddAndExecuteCommand(std::unique_ptr<ICommand>&& command) override;

	bool CanUndo() const override;
	void Undo() override;

	bool CanRedo() const override;
	void Redo() override;

private:
	bool CanMergeWithLastCommand(std::unique_ptr<ICommand>& newCommand);

	static inline const size_t MAX_HISTORY_SIZE = 10;
	size_t m_actionPos = 0;

	std::vector<std::unique_ptr<ICommand>> m_commands;
};