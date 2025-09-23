#pragma once
#include "ICommand.h"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>

namespace UI 
{
	class CommandProcessor
	{
	public:
		CommandProcessor(std::istream& input, std::ostream& output) : m_input(input), m_output(output) {};
		~CommandProcessor() = default;

		void AddItem(const std::string& shortcut,
			const std::string& description,
			std::unique_ptr<ICommand>&& command);
		void Run();
		void ShowInstructions() const;
		void Exit();

	private:
		struct Item
		{
			Item(const std::string& shortcut,
				const std::string& description,
				std::unique_ptr<ICommand>&& command)
				: shortcut(shortcut)
				, description(description)
				, command(std::move(command))
			{
			}

			std::string shortcut;
			std::string description;
			std::unique_ptr<ICommand> command;
		};
		bool ExecuteCommand(const std::string& command);

		std::istream& m_input;
		std::ostream& m_output;

		std::vector<Item> m_items;
		bool m_exit = false;
	};
} // namespace UI