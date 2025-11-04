#pragma once
#include <catch.hpp>
#include <fakeit.hpp>
#include "../NaiveGumballMachine/GumballMachine/GumballMachine.h"

TEST_CASE("Naive GumballMachine")
{
    SECTION("Initial state with no balls")
    {
        naive::GumballMachine machine(0);

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
        naive::GumballMachine machine(5);

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
        naive::GumballMachine machine(3);

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
                    REQUIRE(output.str().find("Quarters returned: 1") != std::string::npos);
                    std::string state = machine.ToString();
                    REQUIRE(state.find("waiting for quarter") != std::string::npos);
                    REQUIRE(state.find("Quarters: 0") != std::string::npos);
                }
            }
        }
    }

    SECTION("Multiple quarter insertion")
    {
        naive::GumballMachine machine(3);

        WHEN("Inserting multiple quarters")
        {
            machine.InsertQuarter(); // First quarter - transitions to HasQuarter
            machine.InsertQuarter(); // Second quarter
            machine.InsertQuarter(); // Third quarter
            machine.InsertQuarter(); // Fourth quarter  
            machine.InsertQuarter(); // Fifth quarter - max
            machine.InsertQuarter(); // Sixth quarter - should be rejected

            THEN("Machine accepts up to 5 quarters and rejects beyond limit")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("waiting for turn of crank") != std::string::npos);
                REQUIRE(state.find("Quarters: 5") != std::string::npos);
            }
        }
    }

    SECTION("Buying gumballs with multiple quarters")
    {
        naive::GumballMachine machine(3);

        WHEN("Buying multiple gumballs with multiple quarters inserted")
        {
            machine.InsertQuarter(); // 1st quarter
            machine.InsertQuarter(); // 2nd quarter
            machine.InsertQuarter(); // 3rd quarter

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
                    REQUIRE(state.find("Quarters: 2") != std::string::npos); // 3-1=2
                }

                AND_WHEN("Turning crank second time")
                {
                    output.str("");
                    output.clear();
                    auto old_cout_buf2 = std::cout.rdbuf(output.rdbuf());

                    machine.TurnCrank();

                    std::cout.rdbuf(old_cout_buf2);

                    THEN("Dispenses second gumball, uses one quarter, remains in has quarter state")
                    {
                        std::string state = machine.ToString();
                        REQUIRE(state.find("waiting for turn of crank") != std::string::npos);
                        REQUIRE(state.find("Inventory: 1 gumball") != std::string::npos);
                        REQUIRE(state.find("Quarters: 1") != std::string::npos); // 2-1=1
                    }

                    AND_WHEN("Turning crank third time")
                    {
                        output.str("");
                        output.clear();
                        auto old_cout_buf3 = std::cout.rdbuf(output.rdbuf());

                        machine.TurnCrank();

                        std::cout.rdbuf(old_cout_buf3);

                        THEN("Dispenses last gumball, transitions to sold out, returns remaining quarters")
                        {
                            REQUIRE(output.str().find("Oops, out of gumballs") != std::string::npos);
                            REQUIRE(output.str().find("Ejecting leftover quarters: 0") == std::string::npos); // No quarters left to eject
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
        naive::GumballMachine machine(2);

        WHEN("Inserting multiple quarters and buying all gumballs")
        {
            machine.InsertQuarter();
            machine.InsertQuarter();
            machine.InsertQuarter(); // 3 quarters total

            machine.TurnCrank(); // Buy first gumball, 2 quarters left
            machine.TurnCrank(); // Buy second gumball, 1 quarter left

            THEN("Machine transitions to sold out and returns remaining quarters")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("sold out") != std::string::npos);
                REQUIRE(state.find("Inventory: 0 gumballs") != std::string::npos);
                REQUIRE(state.find("Quarters: 0") != std::string::npos); // Quarters were returned
            }
        }
    }

    SECTION("Adding quarters during sold state")
    {
        naive::GumballMachine machine(2);

        WHEN("Inserting quarter during gumball dispensing")
        {
            machine.InsertQuarter(); // Go to HasQuarter
            machine.TurnCrank();     // Go to Sold state and dispense

            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.InsertQuarter(); // Try to insert during Sold state

            std::cout.rdbuf(old_cout_buf);

            THEN("Quarter is accepted during sold state")
            {
                REQUIRE(output.str().find("You inserted a quarter") != std::string::npos);
                // After dispense completes, it should transition to HasQuarter because we added a quarter
                std::string state = machine.ToString();
                REQUIRE(state.find("waiting for turn of crank") != std::string::npos);
                REQUIRE(state.find("Quarters: 1") != std::string::npos); // The quarter we just inserted
            }
        }
    }

    SECTION("Ejecting quarters during sold state")
    {
        naive::GumballMachine machine(2);

        WHEN("Ejecting quarters during gumball dispensing")
        {
            machine.InsertQuarter();
            machine.TurnCrank(); // Go to Sold state

            std::stringstream output;
            auto old_cout_buf = std::cout.rdbuf(output.rdbuf());

            machine.EjectQuarter(); // Try to eject during Sold state

            std::cout.rdbuf(old_cout_buf);

            THEN("All quarters are returned and state changes to no quarter")
            {
                std::string state = machine.ToString();
                REQUIRE(state.find("waiting for quarter") != std::string::npos);
            }
        }
    }

    SECTION("Invalid operations")
    {
        naive::GumballMachine machine(2);

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
}