#pragma once
#include "ICommand.h"
#include "../../GumballMachine/GumballMachine.h"

class EjectQuarterCommand : public ICommand
{
public:
	EjectQuarterCommand(GumballMachine& machine) 
		: m_machine(machine)
	{ }

	void Execute(std::istream& input) override
	{
		m_machine.EjectQuarter();
	}

private:
	GumballMachine& m_machine;
};