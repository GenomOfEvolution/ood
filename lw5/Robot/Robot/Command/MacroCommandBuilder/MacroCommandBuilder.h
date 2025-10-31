#pragma once
#include "IMacroCommandBuidler.h"
#include "../../Menu/Menu.h"
#include <iostream>

class MacroCommandBuilder : public IMacroCommandBuidler
{
public:
	MacroCommandBuilder(std::istream& input, Menu& menu);

	std::string GetMacroName() const override;
	std::string GetMacroDescription() const override;
	std::unique_ptr<MacroCommand> BuildMacroCommand() const override;

private:
	Menu& m_menu;
	std::istream& m_input;

	static inline std::string m_END_MACRO = "end_macro";
	static inline std::string m_START_MACRO = "begin_macro";
};