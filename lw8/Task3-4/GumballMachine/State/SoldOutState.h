#pragma once
#include "IState.h"
#include "../GumballMachine/IStateExecutor.h"
#include "../GumballMachine/IGumballMachine.h"
#include <iostream>

class SoldOutState : public IState
{
public:
	SoldOutState(IGumballMachine& machine, IStateExecutor& stateExecutor)
		: m_stateExecutor(stateExecutor)
		, m_machine(machine)
	{	}

	void InsertQuarter() override
	{
		std::cout << "You can't insert a quarter, the machine is sold out\n";
	}

	void EjectQuarter() override
	{
		std::cout << "You can't eject, you haven't inserted a quarter yet\n";
	}

	void TurnCrank() override
	{
		std::cout << "You turned but there's no gumballs\n";
	}

	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}

	std::string ToString() const override
	{
		return "sold out";
	}

	void Refill(unsigned amount) override
	{
		m_machine.AddBalls(amount);
		if (m_machine.GetQuarterCount() > 0 && m_machine.GetBallCount() > 0)
		{
			m_stateExecutor.SetHasQuarterState();
		}
		else
		{
			if (m_machine.GetBallCount() > 0)
			{
				m_stateExecutor.SetNoQuarterState();
			}
		}
	}

private:
	IStateExecutor& m_stateExecutor;
	IGumballMachine& m_machine;
};