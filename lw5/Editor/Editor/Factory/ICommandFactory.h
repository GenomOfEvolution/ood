#pragma once
#include "../Command/ICommand.h"
#include "../Document/IDocument.h"
#include "../Saver/ISaver.h"

#include <memory>
#include <string>

class ICommandFactory
{
public:
	virtual std::unique_ptr<ICommand> CreateCommand(
		IDocument& doc,
		ISaver& saver,
		const std::string& description) = 0;

	virtual ~ICommandFactory() = default;
};