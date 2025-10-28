#pragma once
#include "../Command/ICommand.h"

#include <memory>
#include <string>

class ICommandFactory
{
public:
	virtual std::unique_ptr<ICommand> CreateCommand(const std::string& description) = 0;

	virtual ~ICommandFactory() = default;
};