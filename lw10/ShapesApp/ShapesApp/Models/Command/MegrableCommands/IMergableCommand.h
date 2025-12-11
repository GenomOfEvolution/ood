#pragma once
#include <memory>
#include "../AbstractUndoableCommand.h"

class IMergableCommand : public AbstractUndoableCommand
{
public:
	virtual ~IMergableCommand() = default;
	virtual bool CanMergeWith(const ICommand& other) const = 0;
	virtual void MergeWith(std::unique_ptr<ICommand> other) = 0;
};