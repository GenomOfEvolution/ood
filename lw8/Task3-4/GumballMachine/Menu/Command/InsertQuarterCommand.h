#pragma once
#include "ICommand.h"
#include "../../GumballMachine/GumballMachine.h"

class InsertQuarterCommand : public ICommand
{
public:
	InsertQuarterCommand(GumballMachine& machine)
		: m_machine(machine)
	{
	}

	void Execute(std::istream& input) override
	{
		m_machine.InsertQuarter();
	}

private:
	GumballMachine& m_machine;
};