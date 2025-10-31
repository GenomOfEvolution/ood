#pragma once
#include "ICommand.h"
#include "MacroCommand.h"
#include "../Menu/Menu.h"
#include "MacroCommandBuilder/IMacroCommandBuidler.h"
#include <string>

class MakeMacroCommand : public ICommand
{
public:
	MakeMacroCommand(Menu& menu);
	void Execute() override;
	std::unique_ptr<ICommand> Clone() const override;

private:
	Menu& m_menu;
	std::unique_ptr<IMacroCommandBuidler> m_builder;
};