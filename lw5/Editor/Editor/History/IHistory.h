#pragma once
#include "../Command/ICommand.h"
#include <memory>

class IHistory
{
public:
	virtual void AddAndExecuteCommand(std::unique_ptr<ICommand>&& command) = 0;

	virtual bool CanUndo() const = 0;
	virtual void Undo() = 0;

	virtual bool CanRedo() const = 0;
	virtual void Redo() = 0;
};