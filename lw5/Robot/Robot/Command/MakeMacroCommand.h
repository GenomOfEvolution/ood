#pragma once
#include "ICommand.h"
#include "MacroCommand.h"
#include "../Menu/Menu.h"
#include <string>

class MakeMacroCommand : public ICommand
{
public:
	MakeMacroCommand(Menu& menu);
	void Execute() override;
	std::unique_ptr<ICommand> Clone() const override;

private:
	Menu& m_menu;

	std::string GetMacroName() const;
	std::string GetMacroDescription() const;
	std::unique_ptr<MacroCommand> BuildMacroCommand() const;

	static inline std::string m_END_MACRO = "end_macro";
	static inline std::string m_START_MACRO = "begin_macro";
};