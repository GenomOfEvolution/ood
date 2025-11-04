#pragma once
#include "ICommand.h"
#include "../../GumballMachine/GumballMachine.h"

class RefillCommand : public ICommand
{
public:
	RefillCommand(GumballMachine& machine)
		: m_machine(machine)
	{
	}

	void Execute(std::istream& input) override
	{
		unsigned amount;
		std::cout << "How many gumballs to refill: ";
		input >> amount;

		if (input.bad())
		{
			std::cout << "Wrong number!\n";
			return;
		}

		m_machine.Refill(amount);
	}

private:
	GumballMachine& m_machine;
};