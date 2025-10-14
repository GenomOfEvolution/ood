#pragma once
#include "ICommand.h"
#include "../Robot/Robot.h"

class TurnOffCommand : public ICommand
{
public:
	TurnOffCommand(Robot& robot)
		:m_robot(robot)
	{	}

	void Execute() override
	{
		m_robot.TurnOff();
	}

	std::unique_ptr<ICommand> Clone() const override
	{
		return std::make_unique<TurnOffCommand>(m_robot);
	}

private:
	Robot& m_robot;
};