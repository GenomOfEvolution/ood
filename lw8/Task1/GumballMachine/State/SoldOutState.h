#pragma once
#include "IState.h"
#include "../GumballMachine/IStateExecutor.h"
#include <iostream>

class SoldOutState : public IState
{
public:
	SoldOutState(IStateExecutor& stateExecutor)
		: m_stateExecutor(stateExecutor)
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

private:
	IStateExecutor& m_stateExecutor;
};