#pragma once
#include "IState.h"
#include "../GumballMachine/IStateExecutor.h"
#include <iostream>

class HasQuarterState : public IState
{
public:
	HasQuarterState(IStateExecutor& stateExecutor)
		: m_stateExecutor(stateExecutor)
	{	}

	void InsertQuarter() override
	{
		std::cout << "You can't insert another quarter\n";
	}

	void EjectQuarter() override
	{
		std::cout << "Quarter returned\n";
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

	std::string ToString() const override
	{
		return "waiting for turn of crank";
	}

private:
	IStateExecutor& m_stateExecutor;
};