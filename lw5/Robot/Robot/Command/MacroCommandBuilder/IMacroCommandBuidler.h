#pragma once
#include "../MacroCommand.h"
#include <string>
#include <memory>

class IMacroCommandBuidler
{
public:
	virtual std::string GetMacroName() const = 0;
	virtual std::string GetMacroDescription() const = 0;
	virtual std::unique_ptr<MacroCommand> BuildMacroCommand() const = 0;

	virtual ~IMacroCommandBuidler() = default;
};