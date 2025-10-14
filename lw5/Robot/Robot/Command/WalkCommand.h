#pragma once
#include "ICommand.h"
#include "../Robot/Robot.h"

class WalkCommand : public ICommand
{
public:
	WalkCommand(Robot& robot, WalkDirection direction)
		: m_robot(robot)
		, m_direction(direction)
	{	}

	void Execute() override
	{
		m_robot.Walk(m_direction);
	}

	std::unique_ptr<ICommand> Clone() const override
	{
		return std::make_unique<WalkCommand>(m_robot, m_direction);
	}

private:
	Robot& m_robot;
	WalkDirection m_direction;
};