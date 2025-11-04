#pragma once

class IGumballMachine
{
public:
	virtual void ReleaseBall() = 0;
	virtual unsigned GetBallCount() const = 0;

	virtual ~IGumballMachine() = default;
};