#pragma once

class IGumballMachine
{
public:
	virtual void ReleaseBall() = 0;
	virtual unsigned GetBallCount() const = 0;

	virtual unsigned GetMaxQuarterCount() const = 0;
	virtual unsigned GetQuarterCount() const = 0;
	virtual void AddQuarter() = 0;
	virtual void ReleaseQuarters(unsigned amount) = 0;
	virtual void AddBalls(unsigned amount) = 0;

	virtual ~IGumballMachine() = default;
};