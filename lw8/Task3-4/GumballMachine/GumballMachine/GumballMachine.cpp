#include "GumballMachine.h"
#include <format>
#include <algorithm>

GumballMachine::GumballMachine(unsigned numBalls)
	: m_soldState(*this, *this)
	, m_soldOutState(*this, *this)
	, m_noQuarterState(*this, *this)
	, m_hasQuarterState(*this, *this)
	, m_state(&m_soldOutState)
	, m_count(numBalls)
{
	if (m_count > 0)
	{
		m_state = &m_noQuarterState;
	}
}

void GumballMachine::EjectQuarter()
{
	m_state->EjectQuarter();
}

void GumballMachine::InsertQuarter()
{
	m_state->InsertQuarter();
}

void GumballMachine::TurnCrank()
{
	m_state->TurnCrank();
	m_state->Dispense();
}

std::string GumballMachine::ToString() const
{
	return std::format(
		"Mighty Gumball, Inc.\n"
		"C++-enabled Standing Gumball Model #2016\n"
		"Inventory: {} gumball{}\n"
		"Quarters: {}\n"
		"Machine is {} \n",
		m_count,
		m_count != 1 ? "s" : "",
		m_quarters,
		m_state->ToString());
}

void GumballMachine::Refill(unsigned amount)
{
	m_state->Refill(amount);
}

unsigned GumballMachine::GetBallCount() const
{
	return m_count;
}

unsigned GumballMachine::GetMaxQuarterCount() const
{
	return M_MAX_QUARTERS;
}

unsigned GumballMachine::GetQuarterCount() const
{
	return m_quarters;
}

void GumballMachine::ReleaseBall()
{
	if (m_count != 0)
	{
		std::cout << "A gumball comes rolling out the slot...\n";
		--m_count;
	}
}

void GumballMachine::AddQuarter()
{
	m_quarters++;
}

void GumballMachine::ReleaseQuarters(unsigned amount)
{
	if (m_quarters >= 0)
	{
		m_quarters -= std::min(m_quarters, amount);
	}
}

void GumballMachine::SetSoldOutState()
{
	m_state = &m_soldOutState;
}

void GumballMachine::SetNoQuarterState()
{
	m_state = &m_noQuarterState;
}

void GumballMachine::SetSoldState()
{
	m_state = &m_soldState;
}

void GumballMachine::SetHasQuarterState()
{
	m_state = &m_hasQuarterState;
}

void GumballMachine::AddBalls(unsigned amount)
{
	std::cout << "Adding gumballs...";
	m_count += amount;
}
