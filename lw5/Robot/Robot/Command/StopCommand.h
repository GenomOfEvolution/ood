#pragma once
#include "ICommand.h"
#include "../Robot/Robot.h"

class StopCommand : public ICommand
{
public:
	StopCommand(Robot& robot)
		: m_robot(robot)
	{	}

	void Execute() override
	{
		m_robot.Stop();
	}

	std::unique_ptr<ICommand> Clone() const override
	{
		return std::make_unique<StopCommand>(m_robot);
	}

private:
	Robot& m_robot;
};