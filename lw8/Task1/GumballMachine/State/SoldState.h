#pragma once
#include "IState.h"
#include "../GumballMachine/IStateExecutor.h"
#include "../GumballMachine/IGumballMachine.h"
#include <iostream>

class SoldState : public IState
{
public:
	SoldState(IStateExecutor& stateExecutor, IGumballMachine& gumballMachine)
		: m_stateExecutor(stateExecutor)
		, m_gumballMachine(gumballMachine)
	{	}

	void InsertQuarter() override
	{
		std::cout << "Please wait, we're already giving you a gumball\n";
	}

	void EjectQuarter() override
	{
		std::cout << "Sorry you already turned the crank\n";
	}

	void TurnCrank() override
	{
		std::cout << "Turning twice doesn't get you another gumball\n";
	}

	void Dispense() override
	{
		m_gumballMachine.ReleaseBall();
		if (m_gumballMachine.GetBallCount() == 0)
		{
			std::cout << "Oops, out of gumballs\n";
			m_stateExecutor.SetSoldOutState();
		}
		else
		{
			m_stateExecutor.SetNoQuarterState();
		}
	}

	std::string ToString() const override
	{
		return "delivering a gumball";
	}
private:
	IStateExecutor& m_stateExecutor;
	IGumballMachine& m_gumballMachine;
};