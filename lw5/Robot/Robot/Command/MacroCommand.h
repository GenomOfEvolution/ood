#pragma once
#include "ICommand.h"
#include "../Robot/Robot.h"
#include <vector>

class MacroCommand : public ICommand
{
public:
	void Execute() override
	{
		for (auto& cmd : m_commands)
		{
			cmd->Execute();
		}
	}

	std::unique_ptr<ICommand> Clone() const override
	{
		auto result = std::make_unique<MacroCommand>();

		for (const auto& cmd : m_commands)
		{
			result->AddCommand(cmd->Clone());
		}

		return result;
	}

	void AddCommand(std::unique_ptr<ICommand>&& cmd)
	{
		m_commands.push_back(std::move(cmd));
	}

private:
	std::vector<std::unique_ptr<ICommand>> m_commands;
};