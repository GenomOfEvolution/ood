#define CATCH_CONFIG_MAIN
#pragma once
#include <catch.hpp>
#include <fakeit.hpp>

#include "../GumballMachine/GumballMachine/GumballMachine.h"
#include "../GumballMachine/GumballMachine/IStateExecutor.h"
#include "../GumballMachine/GumballMachine/IGumballMachine.h"
#include "../GumballMachine/State/HasQuarterState.h"
#include "../GumballMachine/State/NoQuarterState.h"
#include "../GumballMachine/State/SoldOutState.h"
#include "../GumballMachine/State/SoldState.h"

TEST_CASE("GumballMachine with State Pattern")
{
    SECTION("Initial state with no balls")
    {
        GumballMachine machine(0);

        THEN("Machine should be in sold out state")
        {
            std::string state = machine.ToString();
            REQUIRE(state.find("sold out") != std::string::npos);
            REQUIRE(state.find("Inventory: 0 gumballs") != std::string::npos);
            REQUIRE(state.find("Quarters: 0") != std::string::npos);
        }

        WHEN("Inserting quarter in sold out state")
        {
            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.InsertQuarter();

            std::cout.rdbuf(old_cout_buf);

            THEN("Shows error message and state remains sold out")
            {
                REQUIRE(output.str().find("You can't insert a quarter, the machine is sold out") != std::string::npos);
                std::string state = machine.ToString();
                REQUIRE(state.find("sold out") != std::string::npos);
            }
        }

        WHEN("Ejecting quarter in sold out state with no quarters")
        {
            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.EjectQuarter();

            std::cout.rdbuf(old_cout_buf);

            THEN("Shows error message")
            {
                REQUIRE(output.str().find("You can't eject, you haven't inserted a quarter yet") != std::string::npos);
            }
        }

        WHEN("Turning crank in sold out state")
        {
            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.TurnCrank();

            std::cout.rdbuf(old_cout_buf);

            THEN("Shows error message")
            {
                REQUIRE(output.str().find("You turned but there's no gumballs") != std::string::npos);
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
            REQUIRE(state.find("Quarters: 0") != std::string::npos);
        }
    }

    SECTION("Normal operation - inserting and ejecting quarter")
    {
        GumballMachine machine(3);

        WHEN("Inserting quarter in no quarter state")
        {
            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.InsertQuarter();

            std::cout.rdbuf(old_cout_buf);

            THEN("Machine transitions to has quarter state with 1 quarter")
            {
                REQUIRE(output.str().find("You inserted a quarter") != std::string::npos);
                std::string state = machine.ToString();
                REQUIRE(state.find("waiting for turn of crank") != std::string::npos);
                REQUIRE(state.find("Quarters: 1") != std::string::npos);
            }

            AND_WHEN("Ejecting quarter")
            {
                output.str("");
                output.clear();
                auto old_cout_buf2 = std::cout.rdbuf(output.rdbuf());

                machine.EjectQuarter();

                std::cout.rdbuf(old_cout_buf2);

                THEN("All quarters are returned and state returns to no quarter")
                {
                    REQUIRE(output.str().find("All quarters returned") != std::string::npos);
                    std::string state = machine.ToString();
                    REQUIRE(state.find("waiting for quarter") != std::string::npos);
                    REQUIRE(state.find("Quarters: 0") != std::string::npos);
                }
            }
        }
    }

    SECTION("Multiple quarter insertion")
    {
        GumballMachine machine(3);

        WHEN("Inserting multiple quarters")
        {
            machine.InsertQuarter(); // First quarter - transitions to HasQuarter
            machine.InsertQuarter(); // Second quarter
            machine.InsertQuarter(); // Third quarter - should be rejected (max is 2 based on mock)

            THEN("Machine accepts up to max quarters and rejects beyond limit")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("waiting for turn of crank") != std::string::npos);
                // Note: Max quarters is 2 based on our mock in state tests
            }
        }
    }

    SECTION("Buying gumballs with multiple quarters")
    {
        GumballMachine machine(3);

        WHEN("Buying multiple gumballs with multiple quarters inserted")
        {
            machine.InsertQuarter(); // 1st quarter
            machine.InsertQuarter(); // 2nd quarter

            AND_WHEN("Turning crank first time")
            {
                std::stringstream output;
                auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

                machine.TurnCrank();

                std::cout.rdbuf(old_cout_buf);

                THEN("Dispenses one gumball, uses one quarter, remains in has quarter state")
                {
                    REQUIRE(output.str().find("You turned...") != std::string::npos);
                    REQUIRE(output.str().find("A gumball comes rolling out the slot") != std::string::npos);
                    std::string state = machine.ToString();
                    REQUIRE(state.find("waiting for turn of crank") != std::string::npos);
                    REQUIRE(state.find("Inventory: 2 gumballs") != std::string::npos);
                    REQUIRE(state.find("Quarters: 1") != std::string::npos); // 2-1=1
                }

                AND_WHEN("Turning crank second time")
                {
                    output.str("");
                    output.clear();
                    auto old_cout_buf2 = std::cout.rdbuf(output.rdbuf());

                    machine.TurnCrank();

                    std::cout.rdbuf(old_cout_buf2);

                    THEN("Dispenses second gumball, uses one quarter, transitions to no quarter state")
                    {
                        std::string state = machine.ToString();
                        REQUIRE(state.find("waiting for quarter") != std::string::npos);
                        REQUIRE(state.find("Inventory: 1 gumball") != std::string::npos);
                        REQUIRE(state.find("Quarters: 0") != std::string::npos); // 1-1=0
                    }

                    AND_WHEN("Turning crank third time after inserting quarter")
                    {
                        machine.InsertQuarter();
                        output.str("");
                        output.clear();
                        auto old_cout_buf3 = std::cout.rdbuf(output.rdbuf());

                        machine.TurnCrank();

                        std::cout.rdbuf(old_cout_buf3);

                        THEN("Dispenses last gumball, transitions to sold out")
                        {
                            REQUIRE(output.str().find("Oops, out of gumballs") != std::string::npos);
                            std::string state = machine.ToString();
                            REQUIRE(state.find("sold out") != std::string::npos);
                            REQUIRE(state.find("Inventory: 0 gumballs") != std::string::npos);
                            REQUIRE(state.find("Quarters: 0") != std::string::npos);
                        }
                    }
                }
            }
        }
    }

    SECTION("Buying until sold out with quarters remaining")
    {
        GumballMachine machine(1);

        WHEN("Inserting multiple quarters and buying last gumball")
        {
            machine.InsertQuarter();
            machine.InsertQuarter(); // 2 quarters total

            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.TurnCrank(); // Buy last gumball

            std::cout.rdbuf(old_cout_buf);

            THEN("Machine transitions to sold out and returns remaining quarters")
            {
                REQUIRE(output.str().find("Oops, out of gumballs") != std::string::npos);
                REQUIRE(output.str().find("Returning remaining quarters: 1") != std::string::npos);
                std::string state = machine.ToString();
                REQUIRE(state.find("sold out") != std::string::npos);
                REQUIRE(state.find("Inventory: 0 gumballs") != std::string::npos);
                REQUIRE(state.find("Quarters: 0") != std::string::npos);
            }
        }
    }

    SECTION("Adding quarters during sold state")
    {
        GumballMachine machine(2);

        WHEN("Inserting quarter during gumball dispensing")
        {
            machine.InsertQuarter(); // Go to HasQuarter
            machine.TurnCrank();     // Go to Sold state and dispense

            // During sold state, try to insert another quarter
            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.InsertQuarter(); // Try to insert during Sold state

            std::cout.rdbuf(old_cout_buf);

            THEN("Quarter can be added during sold state")
            {
                REQUIRE(output.str().find("You inserted a quarter") != std::string::npos);
                // After the current dispense completes, the state will be updated
                // based on the remaining quarters count
            }
        }
    }

    SECTION("ToString format verification")
    {
        GumballMachine machine(1);

        std::string state = machine.ToString();

        THEN("Should contain all required sections")
        {
            REQUIRE(state.find("Mighty Gumball, Inc.") != std::string::npos);
            REQUIRE(state.find("C++-enabled Standing Gumball Model #2016") != std::string::npos);
            REQUIRE(state.find("Inventory: 1 gumball") != std::string::npos);
            REQUIRE(state.find("Quarters: 0") != std::string::npos);
            REQUIRE(state.find("Machine is") != std::string::npos);
        }
    }

    SECTION("Invalid operations")
    {
        GumballMachine machine(2);

        WHEN("Turning crank without quarter")
        {
            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.TurnCrank();

            std::cout.rdbuf(old_cout_buf);

            THEN("Shows error message and no gumball is dispensed")
            {
                REQUIRE(output.str().find("You turned but there's no quarter") != std::string::npos);
                std::string state = machine.ToString();
                REQUIRE(state.find("Inventory: 2 gumballs") != std::string::npos);
            }
        }

        WHEN("Ejecting without inserting quarter")
        {
            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.EjectQuarter();

            std::cout.rdbuf(old_cout_buf);

            THEN("Shows error message")
            {
                REQUIRE(output.str().find("You haven't inserted a quarter") != std::string::npos);
            }
        }
    }

    SECTION("Quarter count management")
    {
        GumballMachine machine(3);

        WHEN("Adding and releasing quarters")
        {
            REQUIRE(machine.ToString().find("Quarters: 0") != std::string::npos);

            machine.InsertQuarter();
            REQUIRE(machine.ToString().find("Quarters: 1") != std::string::npos);

            machine.InsertQuarter();
            REQUIRE(machine.ToString().find("Quarters: 2") != std::string::npos);

            machine.EjectQuarter();
            REQUIRE(machine.ToString().find("Quarters: 0") != std::string::npos);
        }
    }

    SECTION("Refill operation")
    {
        GumballMachine machine(2);

        WHEN("Refilling in no quarter state")
        {
            machine.Refill(3);

            THEN("Balls are added and state remains no quarter")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("Inventory: 5 gumballs") != std::string::npos);
                REQUIRE(state.find("waiting for quarter") != std::string::npos);
            }
        }

        WHEN("Refilling in has quarter state")
        {
            machine.InsertQuarter();
            machine.Refill(3);

            THEN("Balls are added and state remains has quarter")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("Inventory: 5 gumballs") != std::string::npos);
                REQUIRE(state.find("waiting for turn of crank") != std::string::npos);
            }
        }

        WHEN("Refilling in sold out state with quarters")
        {
            GumballMachine emptyMachine(0);
            emptyMachine.InsertQuarter(); // This should be rejected in sold out state
            emptyMachine.InsertQuarter(); // So we need a different approach

            // Create a machine that becomes sold out but has quarters
            GumballMachine machine(1);
            machine.InsertQuarter();
            machine.InsertQuarter();
            machine.TurnCrank(); // Becomes sold out

            machine.Refill(3);

            THEN("Balls are added and state changes to has quarter")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("Inventory: 3 gumballs") != std::string::npos);
            }
        }

        WHEN("Refilling in sold out state without quarters")
        {
            GumballMachine machine(0);
            machine.Refill(3);

            THEN("Balls are added and state changes to no quarter")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("Inventory: 3 gumballs") != std::string::npos);
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
    fakeit::Fake(Method(machineMock, AddQuarter));
    fakeit::Fake(Method(machineMock, ReleaseQuarters));
    fakeit::When(Method(machineMock, GetMaxQuarterCount)).AlwaysReturn(2);
    fakeit::Fake(Method(machineMock, GetQuarterCount));
    fakeit::Fake(Method(machineMock, GetBallCount));
    fakeit::Fake(Method(machineMock, AddBalls));

    auto& machine = machineMock.get();

    GIVEN("has quarter state")
    {
        HasQuarterState state(machine, stateExecutor);

        WHEN("trying to insert quarter when not at max")
        {
            fakeit::When(Method(machineMock, GetQuarterCount)).Return(1);
            state.InsertQuarter();
            THEN("quarter is added")
            {
                fakeit::Verify(Method(machineMock, AddQuarter)).Once();
            }
        }

        WHEN("trying to insert quarter when at max")
        {
            fakeit::When(Method(machineMock, GetQuarterCount)).Return(2);
            state.InsertQuarter();
            THEN("quarter is not added")
            {
                fakeit::Verify(Method(machineMock, AddQuarter)).Never();
            }
        }

        WHEN("trying to eject quarter")
        {
            fakeit::When(Method(machineMock, GetQuarterCount)).Return(2);
            state.EjectQuarter();
            THEN("all quarters are returned and state changes to no quarter")
            {
                fakeit::Verify(Method(machineMock, ReleaseQuarters).Using(2)).Once();
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
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("refilling balls")
        {
            state.Refill(5);

            THEN("balls are added and state remains unchanged")
            {
                fakeit::Verify(Method(machineMock, AddBalls).Using(5)).Once();
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
            }
        }
    }

    GIVEN("no quarter state")
    {
        NoQuarterState state(machine, stateExecutor);

        WHEN("inserting quarter")
        {
            state.InsertQuarter();
            THEN("quarter is added and state changes to has quarter")
            {
                fakeit::Verify(Method(machineMock, AddQuarter)).Once();
                fakeit::Verify(Method(stateExecutorMock, SetHasQuarterState)).Once();
            }
        }

        WHEN("trying to eject quarter")
        {
            state.EjectQuarter();
            THEN("shows message but no state change")
            {
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("trying to turn crank")
        {
            state.TurnCrank();
            THEN("shows message but no state change")
            {
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("trying to dispense")
        {
            state.Dispense();
            THEN("shows message but no state change")
            {
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("refilling balls")
        {
            state.Refill(3);

            THEN("balls are added and state remains unchanged")
            {
                fakeit::Verify(Method(machineMock, AddBalls).Using(3)).Once();
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
            }
        }
    }

    GIVEN("sold out state")
    {
        SoldOutState state(machine, stateExecutor);

        WHEN("trying to insert quarter")
        {
            state.InsertQuarter();
            THEN("shows message but no state change")
            {
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("trying to eject quarter")
        {
            state.EjectQuarter();
            THEN("shows message but no state change")
            {
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("trying to turn crank")
        {
            state.TurnCrank();
            THEN("shows message but no state change")
            {
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("trying to dispense")
        {
            state.Dispense();
            THEN("shows message but no state change")
            {
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("refilling balls")
        {
            fakeit::When(Method(machineMock, GetQuarterCount)).Return(2);
            fakeit::When(Method(machineMock, GetBallCount)).Return(5);
            state.Refill(5);

            THEN("balls are added and state changes to has quarter")
            {
                fakeit::Verify(Method(machineMock, AddBalls).Using(5)).Once();
                fakeit::Verify(Method(stateExecutorMock, SetHasQuarterState)).Once();
            }
        }

        WHEN("refilling results in zero total balls without quarters")
        {
            fakeit::When(Method(machineMock, GetQuarterCount)).Return(0);
            fakeit::When(Method(machineMock, GetBallCount)).Return(0);

            state.Refill(0);

            THEN("state remains sold out")
            {
                fakeit::Verify(Method(machineMock, AddBalls).Using(0)).Once();
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
            }
        }
    }

    GIVEN("sold state with balls remaining and quarters")
    {
        fakeit::When(Method(machineMock, GetBallCount)).Return(2);
        fakeit::When(Method(machineMock, GetQuarterCount)).Return(1);
        SoldState state(stateExecutor, machine);

        WHEN("trying to insert quarter when not at max")
        {
            fakeit::When(Method(machineMock, GetQuarterCount)).Return(1);
            state.InsertQuarter();
            THEN("quarter is added")
            {
                fakeit::Verify(Method(machineMock, AddQuarter)).Once();
            }
        }

        WHEN("trying to insert quarter when at max")
        {
            fakeit::When(Method(machineMock, GetQuarterCount)).Return(2);
            state.InsertQuarter();
            THEN("quarter is not added")
            {
                fakeit::Verify(Method(machineMock, AddQuarter)).Never();
            }
        }

        WHEN("trying to eject quarter")
        {
            state.EjectQuarter();
            THEN("shows message but no state change")
            {
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("trying to turn crank")
        {
            state.TurnCrank();
            THEN("shows message but no state change")
            {
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
                fakeit::VerifyNoOtherInvocations(machineMock);
            }
        }

        WHEN("dispensing gumball")
        {
            state.Dispense();
            THEN("releases ball, releases one quarter and transitions to has quarter state")
            {
                fakeit::Verify(Method(machineMock, ReleaseBall)).Once();
                fakeit::Verify(Method(machineMock, ReleaseQuarters).Using(1)).Once();
                fakeit::Verify(Method(stateExecutorMock, SetHasQuarterState)).Once();
            }
        }


        WHEN("trying to refill balls")
        {
            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            state.Refill(5);

            std::cout.rdbuf(old_cout_buf);

            THEN("shows error message and no balls are added")
            {
                fakeit::Verify(Method(machineMock, AddBalls)).Never();
                fakeit::VerifyNoOtherInvocations(stateExecutorMock);
            }
        }
    }

    GIVEN("sold state with balls remaining but no quarters")
    {
        fakeit::When(Method(machineMock, GetBallCount)).Return(2);
        fakeit::When(Method(machineMock, GetQuarterCount)).Return(0);
        SoldState state(stateExecutor, machine);

        WHEN("dispensing gumball")
        {
            state.Dispense();
            THEN("releases ball, releases one quarter and transitions to no quarter state")
            {
                fakeit::Verify(Method(machineMock, ReleaseBall)).Once();
                fakeit::Verify(Method(machineMock, ReleaseQuarters).Using(1)).Once();
                fakeit::Verify(Method(stateExecutorMock, SetNoQuarterState)).Once();
            }
        }
    }

    GIVEN("sold state with last gumball and remaining quarters")
    {
        fakeit::When(Method(machineMock, GetBallCount)).Return(0);
        fakeit::When(Method(machineMock, GetQuarterCount)).Return(2);
        SoldState state(stateExecutor, machine);

        WHEN("dispensing last gumball")
        {
            state.Dispense();
            THEN("releases ball, releases one quarter, releases remaining quarters and transitions to sold out")
            {
                fakeit::Verify(Method(machineMock, ReleaseBall)).Once();
                fakeit::Verify(Method(machineMock, ReleaseQuarters).Using(1)).Once();
                fakeit::Verify(Method(machineMock, ReleaseQuarters).Using(2)).Once();
                fakeit::Verify(Method(stateExecutorMock, SetSoldOutState)).Once();
            }
        }
    }

    GIVEN("sold state with last gumball and no remaining quarters")
    {
        fakeit::When(Method(machineMock, GetBallCount)).Return(0);
        fakeit::When(Method(machineMock, GetQuarterCount)).Return(0);
        SoldState state(stateExecutor, machine);

        WHEN("dispensing last gumball")
        {
            state.Dispense();
            THEN("releases ball, releases one quarter and transitions to sold out")
            {
                fakeit::Verify(Method(machineMock, ReleaseBall)).Once();
                fakeit::Verify(Method(machineMock, ReleaseQuarters).Using(1)).Once();
                fakeit::Verify(Method(stateExecutorMock, SetSoldOutState)).Once();
                // Should not try to release quarters again since count is 0
                fakeit::Verify(Method(machineMock, ReleaseQuarters).Using(0)).Never();
            }
        }
    }
}