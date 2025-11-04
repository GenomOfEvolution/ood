#pragma once
#include "ICommand.h"
#include "../../GumballMachine/GumballMachine.h"

class ToStringCommand : public ICommand
{
public:
	ToStringCommand(GumballMachine& machine)
		: m_machine(machine)
	{
	}

	void Execute(std::istream& input) override
	{
		std::cout << m_machine.ToString() << std::endl;
	}

private:
	GumballMachine& m_machine;
};