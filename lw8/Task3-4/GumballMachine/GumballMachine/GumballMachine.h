#pragma once
#include "IGumballMachine.h"
#include "IStateExecutor.h"
#include "../State/IState.h"
#include "../State/HasQuarterState.h"
#include "../State/NoQuarterState.h"
#include "../State/SoldOutState.h"
#include "../State/SoldState.h"

class GumballMachine : private IStateExecutor, private IGumballMachine
{
public:
	GumballMachine(unsigned numBalls);

	void EjectQuarter();
	void InsertQuarter();
	void TurnCrank();
	void Refill(unsigned amount);
	std::string ToString() const;

private:
	unsigned GetBallCount() const override;
	unsigned GetMaxQuarterCount() const override;
	unsigned GetQuarterCount() const override;
	void ReleaseBall() override;
	void AddQuarter() override;
	void ReleaseQuarters(unsigned amount) override;

	void SetSoldOutState() override;
	void SetNoQuarterState() override;
	void SetSoldState() override;
	void SetHasQuarterState() override;
	void AddBalls(unsigned amount) override;

private:
	static inline const unsigned M_MAX_QUARTERS = 5;
	unsigned m_count = 0;
	unsigned m_quarters = 0;
	IState* m_state;

	SoldState m_soldState;
	SoldOutState m_soldOutState;
	NoQuarterState m_noQuarterState;
	HasQuarterState m_hasQuarterState;
};