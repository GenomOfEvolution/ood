#pragma once
#include "IState.h"
#include "../GumballMachine/IStateExecutor.h"
#include "../GumballMachine/IGumballMachine.h"
#include <iostream>

class NoQuarterState : public IState
{
public:
	NoQuarterState(IGumballMachine& machine, IStateExecutor& stateExecutor)
		: m_stateExecutor(stateExecutor)
		, m_machine(machine)
	{	}

	void InsertQuarter() override
	{
		m_machine.AddQuarter();
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
	IGumballMachine& m_machine;
};