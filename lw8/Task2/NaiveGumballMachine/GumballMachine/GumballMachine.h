#pragma once
#include <iostream>
#include <format>

namespace naive
{
    class GumballMachine
    {
    public:
        enum class State
        {
            SoldOut,
            NoQuarter,
            HasQuarter,
            Sold,
        };

        GumballMachine(const unsigned count)
            : m_ballCount(count)
            , m_state(count > 0 ? State::NoQuarter : State::SoldOut)
        {
        }

        void InsertQuarter()
        {
            using namespace std;
            switch (m_state)
            {
            case State::SoldOut:
                cout << "You can't insert a quarter, the machine is sold out" << endl;
                break;
            case State::NoQuarter:
                if (m_quarterCount < MAX_QUARTER_COUNT)
                {
                    cout << "You inserted a quarter" << endl;
                    m_quarterCount = 1;
                    m_state = State::HasQuarter;
                }
                else
                {
                    cout << "You can't insert more than " << MAX_QUARTER_COUNT << " quarters" << endl;
                }
                break;
            case State::HasQuarter:
                if (m_quarterCount < MAX_QUARTER_COUNT)
                {
                    cout << "You inserted a quarter" << endl;
                    ++m_quarterCount;
                }
                else
                {
                    cout << "You can't insert more than " << MAX_QUARTER_COUNT << " quarters" << endl;
                }
                break;
            case State::Sold:
                if (m_quarterCount < MAX_QUARTER_COUNT)
                {
                    cout << "You inserted a quarter" << endl;
                    ++m_quarterCount;
                }
                else
                {
                    cout << "You can't insert more than " << MAX_QUARTER_COUNT << " quarters" << endl;
                }
                break;
            }
        }

        void EjectQuarter()
        {
            using namespace std;
            switch (m_state)
            {
            case State::HasQuarter:
            case State::Sold: // Разрешаем возврат даже во время выдачи
                cout << "Quarters returned: " << m_quarterCount << endl;
                m_quarterCount = 0;
                m_state = State::NoQuarter;
                break;
            case State::NoQuarter:
                cout << "You haven't inserted a quarter" << endl;
                break;
            case State::SoldOut:
                if (m_quarterCount > 0)
                {
                    cout << "Quarters returned: " << m_quarterCount << endl;
                    m_quarterCount = 0;
                }
                else
                {
                    cout << "You can't eject, you haven't inserted a quarter yet" << endl;
                }
                break;
            }
        }

        void TurnCrank()
        {
            using namespace std;
            switch (m_state)
            {
            case State::SoldOut:
                cout << "You turned but there's no gumballs" << endl;
                break;
            case State::NoQuarter:
                cout << "You turned but there's no quarter" << endl;
                break;
            case State::HasQuarter:
                cout << "You turned..." << endl;
                --m_quarterCount;
                m_state = State::Sold;
                Dispense();
                break;
            case State::Sold:
                cout << "Turning twice doesn't get you another gumball" << endl;
                break;
            }
        }

        std::string ToString() const
        {
            using namespace std;
            return format(
                "Mighty Gumball, Inc.\n"
                "C++-enabled Standing Gumball Model #2016\n"
                "Inventory: {} gumball{}\n"
                "Quarters: {}\n"
                "Machine is {}\n",
                m_ballCount,
                m_ballCount != 1 ? "s" : "",
                m_quarterCount,
                StateToString());
        }

    private:
        void Dispense()
        {
            using namespace std;
            switch (m_state)
            {
            case State::Sold:
                cout << "A gumball comes rolling out the slot" << endl;
                --m_ballCount;

                if (m_ballCount == 0)
                {
                    cout << "Oops, out of gumballs" << endl;
                    // Возвращаем все оставшиеся монетки
                    if (m_quarterCount > 0)
                    {
                        cout << "Ejecting leftover quarters: " << m_quarterCount << endl;
                       m_quarterCount = 0;
                    }
                    m_state = State::SoldOut;
                }
                else
                {
                    // После выдачи шарика проверяем, остались ли монетки
                    if (m_quarterCount > 0)
                    {
                        m_state = State::HasQuarter;
                    }
                    else
                    {
                        m_state = State::NoQuarter;
                    }
                }
                break;
            case State::NoQuarter:
                cout << "You need to pay first" << endl;
                break;
            case State::SoldOut:
            case State::HasQuarter:
                cout << "No gumball dispensed" << endl;
                break;
            }
        }

        std::string StateToString() const
        {
            switch (m_state)
            {
            case State::SoldOut:
                return "sold out";
            case State::NoQuarter:
                return "waiting for quarter";
            case State::HasQuarter:
                return "waiting for turn of crank";
            case State::Sold:
                return "delivering a gumball";
            default:
                return "";
            }
        }

        static const unsigned MAX_QUARTER_COUNT = 5;
        unsigned m_ballCount = 0;
        unsigned m_quarterCount = 0;
        State m_state = State::SoldOut;
    };
} // naive