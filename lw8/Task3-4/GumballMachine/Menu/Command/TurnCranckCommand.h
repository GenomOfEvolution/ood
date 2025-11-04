#pragma once
#include "ICommand.h"
#include "../../GumballMachine/GumballMachine.h"

class TurnCranckCommand : public ICommand
{
public:
	TurnCranckCommand(GumballMachine& machine)
		: m_machine(machine)
	{
	}

	void Execute(std::istream& input) override
	{
		m_machine.TurnCrank();
	}

private:
	GumballMachine& m_machine;
};