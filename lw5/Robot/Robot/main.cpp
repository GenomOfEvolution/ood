#include "Menu/Menu.h"
#include "Robot/Robot.h"

#include "Command/MacroCommand.h"
#include "Command/MenuExitCommand.h"
#include "Command/MenuHelpCommand.h"
#include "Command/StopCommand.h"
#include "Command/TurnOffCommand.h"
#include "Command/TurnOnCommand.h"
#include "Command/WalkCommand.h"
#include "Command/MakeMacroCommand.h"

// TODO: убрать логику сборки макрокоманды в отдельный класс
// чтобы MakeMacroCommand была тонкой

int main()
{
	Robot robot;
	Menu menu;
	menu.AddItem("on", "Turns the Robot on",
		std::make_unique<TurnOnCommand>(robot));
	menu.AddItem("off", "Turns the Robot off",
		std::make_unique<TurnOffCommand>(robot));

	menu.AddItem("north", "Makes the Robot walk north",
		std::make_unique<WalkCommand>(robot, WalkDirection::North));
	menu.AddItem("south", "Makes the Robot walk south",
		std::make_unique<WalkCommand>(robot, WalkDirection::South));
	menu.AddItem("west", "Makes the Robot walk west",
		std::make_unique<WalkCommand>(robot, WalkDirection::West));
	menu.AddItem("east", "Makes the Robot walk east",
		std::make_unique<WalkCommand>(robot, WalkDirection::East));
	menu.AddItem("stop", "Stops the Robot",
		std::make_unique<StopCommand>(robot));
	auto cmd = std::make_unique<MacroCommand>();
	cmd->AddCommand(std::make_unique<TurnOnCommand>(robot));
	cmd->AddCommand(std::make_unique<WalkCommand>(robot, WalkDirection::North));
	cmd->AddCommand(std::make_unique<WalkCommand>(robot, WalkDirection::East));
	cmd->AddCommand(std::make_unique<WalkCommand>(robot, WalkDirection::South));
	cmd->AddCommand(std::make_unique<WalkCommand>(robot, WalkDirection::West));
	cmd->AddCommand(std::make_unique<TurnOffCommand>(robot));
	menu.AddItem("patrol", "Patrol the territory", std::move(cmd));

	menu.AddItem("begin_macro", "Make your own command",
		std::make_unique<MakeMacroCommand>(menu));

	menu.AddItem("help", "Show instructions",
		std::make_unique<MenuHelpCommand>(menu));
	menu.AddItem("exit", "Exit from this menu",
		std::make_unique<ExitMenuCommand>(menu));

	menu.Run();

	return EXIT_SUCCESS;
}