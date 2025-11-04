#pragma once
#include "IState.h"
#include "../GumballMachine/IStateExecutor.h"
#include "../GumballMachine/IGumballMachine.h"
#include <iostream>

class HasQuarterState : public IState
{
public:
	HasQuarterState(IGumballMachine& machine, IStateExecutor& stateExecutor)
		: m_stateExecutor(stateExecutor)
		, m_machine(machine)
	{	}

	void InsertQuarter() override
	{
		if (m_machine.GetQuarterCount() >= m_machine.GetMaxQuarterCount())
		{
			std::cout << "You can't insert another quarter\n";
			return;
		}

		m_machine.AddQuarter();
		std::cout << "You inserted a quarter\n";
	}

	void EjectQuarter() override
	{
		std::cout << "All quarters returned\n";
		m_machine.ReleaseQuarters(m_machine.GetMaxQuarterCount());
		m_stateExecutor.SetNoQuarterState();
	}

	void TurnCrank() override
	{
		std::cout << "You turned...\n";
		m_stateExecutor.SetSoldState();
	}

	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}

	void Refill(unsigned amount) override
	{
		m_machine.AddBalls(amount);
	}

	std::string ToString() const override
	{
		return "waiting for turn of crank";
	}

private:
	IStateExecutor& m_stateExecutor;
	IGumballMachine& m_machine;
};