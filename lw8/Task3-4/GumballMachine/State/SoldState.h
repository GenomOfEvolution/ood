#pragma once
#include "IState.h"
#include "../GumballMachine/IGumballMachine.h"
#include "../GumballMachine/IStateExecutor.h"

class SoldState : public IState
{
public:
    SoldState(IStateExecutor& stateExecutor, IGumballMachine& gumballMachine)
        : m_stateExecutor(stateExecutor)
        , m_gumballMachine(gumballMachine)
    {
    }

    void InsertQuarter() override
    {
        if (m_gumballMachine.GetQuarterCount() < m_gumballMachine.GetMaxQuarterCount())
        {
            m_gumballMachine.AddQuarter();
            std::cout << "You inserted a quarter\n";
        }
        else
        {
            std::cout << "You can't insert more than " << m_gumballMachine.GetMaxQuarterCount() << " quarters\n";
        }
    }

    void EjectQuarter() override
    {
        std::cout << "Sorry you already turned the crank\n";
    }

    void TurnCrank() override
    {
        std::cout << "Turning twice doesn't get you another gumball\n";
    }

    void Dispense() override
    {
        m_gumballMachine.ReleaseBall();
        m_gumballMachine.ReleaseQuarters(1);

        if (m_gumballMachine.GetBallCount() == 0)
        {
            std::cout << "Oops, out of gumballs\n";

            unsigned remainingQuarters = m_gumballMachine.GetQuarterCount();
            if (remainingQuarters > 0)
            {
                std::cout << "Returning remaining quarters: " << remainingQuarters << std::endl;
                m_gumballMachine.ReleaseQuarters(remainingQuarters);
            }
            m_stateExecutor.SetSoldOutState();
        }
        else
        {
            if (m_gumballMachine.GetQuarterCount() > 0)
            {
                m_stateExecutor.SetHasQuarterState();
            }
            else
            {
                m_stateExecutor.SetNoQuarterState();
            }
        }
    }

    void Refill(unsigned amount) override
    {
        std::cout << "Can't refill while disposing a gumball\n";
    }

    std::string ToString() const override
    {
        return "delivering a gumball";
    }

private:
    IStateExecutor& m_stateExecutor;
    IGumballMachine& m_gumballMachine;
};