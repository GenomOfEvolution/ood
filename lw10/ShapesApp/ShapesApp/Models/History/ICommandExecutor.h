#pragma once
#include "../Command/ICommand.h"
#include <memory>

class ICommandExecutor
{
public:
	virtual void AddAndExecuteCommand(std::unique_ptr<ICommand>&& command) = 0;
	virtual void Clear() = 0;
	virtual ~ICommandExecutor() = default;
};
