#pragma once
#include <memory>

class ICommand
{
public:
	virtual void Execute() = 0;
	virtual ~ICommand() = default;
	virtual std::unique_ptr<ICommand> Clone() const = 0;
};