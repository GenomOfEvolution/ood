#pragma once
#include "ICommand.h"

class IUndoableCommand : public ICommand
{
public:
	virtual void Unexecute() = 0;
	virtual ~IUndoableCommand() = default;
};