#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <fakeit.hpp>
#include <sstream>

#include "../GumballMachine/GumballMachine/GumballMachine.h"
#include "../GumballMachine/State/HasQuarterState.h"
#include "../GumballMachine/State/NoQuarterState.h"
#include "../GumballMachine/State/SoldOutState.h"
#include "../GumballMachine/State/SoldState.h"

TEST_CASE("GumballMachine")
{
    SECTION("Initial state with no balls")
    {
        GumballMachine machine(0);

        THEN("Machine should be in sold out state")
        {
            std::string state = machine.ToString();
            REQUIRE(state.find("sold out") != std::string::npos);
            REQUIRE(state.find("Inventory: 0 gumballs") != std::string::npos);
        }

        WHEN("Inserting quarter in sold out state")
        {
            machine.InsertQuarter();
            THEN("State remains sold out")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("sold out") != std::string::npos);
            }
        }

        WHEN("Turning crank in sold out state")
        {
            machine.TurnCrank();
            THEN("State remains sold out")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("sold out") != std::string::npos);
            }
        }

        WHEN("Ejecting quarter in sold out state")
        {
            machine.EjectQuarter();
            THEN("State remains sold out")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("sold out") != std::string::npos);
            }
        }
    }

    SECTION("Initial state with balls")
    {
        GumballMachine machine(5);

        THEN("Machine should be in no quarter state")
        {
            std::string state = machine.ToString();
            REQUIRE(state.find("waiting for quarter") != std::string::npos);
            REQUIRE(state.find("Inventory: 5 gumballs") != std::string::npos);
        }
    }

    SECTION("Normal operation with multiple balls")
    {
        GumballMachine machine(3);

        WHEN("Inserting quarter")
        {
            machine.InsertQuarter();

            THEN("Machine transitions to has quarter state")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("waiting for turn of crank") != std::string::npos);
            }

            AND_WHEN("Ejecting quarter")
            {
                machine.EjectQuarter();

                THEN("Machine returns to no quarter state")
                {
                    std::string state = machine.ToString();
                    REQUIRE(state.find("waiting for quarter") != std::string::npos);
                }
            }

            AND_WHEN("Turning crank")
            {
                machine.TurnCrank();

                THEN("Machine dispenses ball and returns to no quarter state")
                {
                    std::string state = machine.ToString();
                    REQUIRE(state.find("waiting for quarter") != std::string::npos);
                    REQUIRE(state.find("Inventory: 2 gumballs") != std::string::npos);
                }
            }
        }
    }

    SECTION("Operation until sold out")
    {
        GumballMachine machine(2);

        WHEN("Buying all balls")
        {
            machine.InsertQuarter();
            machine.TurnCrank(); // 1 ball left

            machine.InsertQuarter();
            machine.TurnCrank(); // 0 balls left

            THEN("Machine should be sold out")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("sold out") != std::string::npos);
                REQUIRE(state.find("Inventory: 0 gumballs") != std::string::npos);
            }

            AND_WHEN("Trying to insert quarter in sold out")
            {
                machine.InsertQuarter();

                THEN("Remains sold out")
                {
                    std::string state = machine.ToString();
                    REQUIRE(state.find("sold out") != std::string::npos);
                }
            }
        }
    }

    SECTION("Multiple quarter insertion prevention")
    {
        GumballMachine machine(2);

        WHEN("Inserting quarter twice")
        {
            machine.InsertQuarter();
            machine.InsertQuarter(); 

            THEN("Machine stays in has quarter state")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("waiting for turn of crank") != std::string::npos);
            }

            AND_WHEN("Ejecting quarter after multiple inserts")
            {
                machine.EjectQuarter();

                THEN("Machine returns to no quarter state")
                {
                    std::string state = machine.ToString();
                    REQUIRE(state.find("waiting for quarter") != std::string::npos);
                }
            }
        }
    }

    SECTION("ToString format verification")
    {
        GumballMachine machine(1);

        std::string state = machine.ToString();

        THEN("Should contain company header")
        {
            REQUIRE(state.find("Mighty Gumball, Inc.") != std::string::npos);
        }

        THEN("Should contain model info")
        {
            REQUIRE(state.find("C++-enabled Standing Gumball Model #2016") != std::string::npos);
        }

        THEN("Should contain correct inventory")
        {
            REQUIRE(state.find("Inventory: 1 gumball") != std::string::npos);
        }

        THEN("Should contain state description")
        {
            REQUIRE(state.find("Machine is") != std::string::npos);
        }
    }

    SECTION("Single ball operation")
    {
        GumballMachine machine(1);

        WHEN("Buying the last ball")
        {
            machine.InsertQuarter();
            machine.TurnCrank();

            THEN("Machine transitions to sold out")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("sold out") != std::string::npos);
                REQUIRE(state.find("Inventory: 0 gumballs") != std::string::npos);
            }
        }
    }

    SECTION("Invalid operations in no quarter state")
    {
        GumballMachine machine(2);

        WHEN("Turning crank without quarter")
        {
            machine.TurnCrank();

            THEN("Machine remains in no quarter state")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("waiting for quarter") != std::string::npos);
            }
        }

        WHEN("Ejecting quarter without inserting")
        {
            machine.EjectQuarter();

            THEN("Machine remains in no quarter state")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("waiting for quarter") != std::string::npos);
            }
        }
    }
}

TEST_CASE("States")
{
	fakeit::Mock<IStateExecutor> stateExecutorMock;

	fakeit::Fake(Method(stateExecutorMock, SetSoldOutState));
	fakeit::Fake(Method(stateExecutorMock, SetNoQuarterState));
	fakeit::Fake(Method(stateExecutorMock, SetSoldState));
	fakeit::Fake(Method(stateExecutorMock, SetHasQuarterState));

	auto& stateExecutor = stateExecutorMock.get();

	fakeit::Mock<IGumballMachine> machineMock;

	fakeit::Fake(Method(machineMock, ReleaseBall));

	auto& machine = machineMock.get();

	GIVEN("has quarter state")
	{
		HasQuarterState state(stateExecutor);

		WHEN("trying to insert quarter")
		{
			state.InsertQuarter();
			THEN("nothing happens")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
		WHEN("trying to eject quarter")
		{
			state.EjectQuarter();
			THEN("quarter ejects from machine")
			{
				fakeit::Verify(Method(stateExecutorMock, SetNoQuarterState)).Once();
			}
		}
		WHEN("trying to turn crank")
		{
			state.TurnCrank();
			THEN("machine sets to the sold state")
			{
				fakeit::Verify(Method(stateExecutorMock, SetSoldState)).Once();
			}
		}
		WHEN("trying to dispense")
		{
			state.Dispense();
			THEN("nothing happens")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
	}

	GIVEN("no quarter state")
	{
		NoQuarterState state(stateExecutor);

		WHEN("inserting quarter")
		{
			state.InsertQuarter();
			THEN("machine accepts quarter and changes state")
			{
				fakeit::Verify(Method(stateExecutorMock, SetHasQuarterState)).Once();
			}
		}
		WHEN("trying to eject quarter")
		{
			state.EjectQuarter();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
		WHEN("trying to turn crank")
		{
			state.TurnCrank();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
		WHEN("trying to dispense")
		{
			state.Dispense();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
	}

	GIVEN("sold out state")
	{
		SoldOutState state(stateExecutor);

		WHEN("trying to insert quarter")
		{
			state.InsertQuarter();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
		WHEN("trying to eject quarter")
		{
			state.EjectQuarter();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
		WHEN("trying to turn crank")
		{
			state.TurnCrank();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
		WHEN("trying to dispense")
		{
			state.Dispense();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
	}

	GIVEN("sold state with gumballs remaining")
	{
		fakeit::When(Method(machineMock, GetBallCount)).Return(1);
		SoldState state(stateExecutor, machine);

		WHEN("trying to insert quarter")
		{
			state.InsertQuarter();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
		WHEN("trying to eject quarter")
		{
			state.EjectQuarter();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
		WHEN("trying to turn crank")
		{
			state.TurnCrank();
			THEN("shows message but no state change")
			{
				fakeit::VerifyNoOtherInvocations(stateExecutorMock);
			}
		}
		WHEN("dispensing gumball")
		{
			state.Dispense();
			THEN("releases ball and transitions to no quarter state")
			{
				fakeit::Verify(Method(machineMock, ReleaseBall)).Once();
				fakeit::Verify(Method(stateExecutorMock, SetNoQuarterState)).Once();
			}
		}
	}

	GIVEN("sold state with last gumball")
	{
		fakeit::When(Method(machineMock, GetBallCount)).Return(0);
		SoldState state(stateExecutor, machine);

		WHEN("dispensing last gumball")
		{
			state.Dispense();
			THEN("releases ball and transitions to sold out state")
			{
				fakeit::Verify(Method(machineMock, ReleaseBall)).Once();
				fakeit::Verify(Method(stateExecutorMock, SetSoldOutState)).Once();
			}
		}
	}
}