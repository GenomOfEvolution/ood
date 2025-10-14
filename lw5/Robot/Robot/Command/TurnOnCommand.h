#pragma once
#include "ICommand.h"
#include "../Robot/Robot.h"

class TurnOnCommand : public ICommand
{
public:
	TurnOnCommand(Robot& robot)
		: m_robot(robot)
	{	}

	void Execute() override
	{
		m_robot.TurnOn();
	}

	std::unique_ptr<ICommand> Clone() const override
	{
		return std::make_unique<TurnOnCommand>(m_robot);
	}

private:
	Robot& m_robot;
};