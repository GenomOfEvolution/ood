#include <iostream>
#include "GumballMachine/GumballMachine.h"
#include "Menu/Menu.h"

#include "Menu/Command/EjectQuarterCommand.h"
#include "Menu/Command/ExitCommand.h"
#include "Menu/Command/HelpCommand.h"
#include "Menu/Command/InsertQuarterCommand.h"
#include "Menu/Command/ToStringCommand.h"
#include "Menu/Command/TurnCranckCommand.h"
#include "Menu/Command/RefillCommand.h"

int main()
{
	GumballMachine machine(0);
	Menu menu;

	menu.AddItem("Help", "Show this menu",
		std::make_unique<HelpCommand>(menu));
	menu.AddItem("Exit", "Exit from this menu",
		std::make_unique<ExitCommand>(menu));
	menu.AddItem("EjectQuarter", "Eject all quarters from gumball machine",
		std::make_unique<EjectQuarterCommand>(machine));
	menu.AddItem("InsertQuarter", "Insert single quarter to gumball machine",
		std::make_unique<InsertQuarterCommand>(machine));
	menu.AddItem("TurnCranck", "Turn cracnk one time",
		std::make_unique<TurnCranckCommand>(machine));
	menu.AddItem("Refill", "Refill <amount> of gumballs", 
		std::make_unique<RefillCommand>(machine));
	menu.AddItem("Info", "Show state of gumball machine",
		std::make_unique<ToStringCommand>(machine));

	menu.Run();

	return EXIT_SUCCESS;
}