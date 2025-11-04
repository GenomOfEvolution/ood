#pragma once
#include "IState.h"
#include "../GumballMachine/IStateExecutor.h"
#include <iostream>

class NoQuarterState : public IState
{
public:
	NoQuarterState(IStateExecutor& stateExecutor)
		: m_stateExecutor(stateExecutor)
	{	}

	void InsertQuarter() override
	{
		std::cout << "You inserted a quarter\n";
		m_stateExecutor.SetHasQuarterState();
	}

	void EjectQuarter() override
	{
		std::cout << "You haven't inserted a quarter\n";
	}

	void TurnCrank() override
	{
		std::cout << "You turned but there's no quarter\n";
	}

	void Dispense() override
	{
		std::cout << "You need to pay first\n";
	}

	std::string ToString() const override
	{
		return "waiting for quarter";
	}

private:
	IStateExecutor& m_stateExecutor;
};