#include "pch.h"
#include <gtest/gtest.h>
#include "../DuckDance/lib/Duck/ModelDuck.h"
#include "../DuckDance/lib/Dance/DanceBehavior.h"
#include "../DuckDance/lib/Fly/FlyBehavior.h"
#include "../DuckDance/lib/Quack/QuakBehavior.h"

TEST(TestDuckLambda, TestDoubleQuack)
{
    int quackCounter = 0;
    int flyCounter = 0;

    auto mockQuack = [&quackCounter]() mutable { quackCounter++; };
    auto mockFly = [&flyCounter, &mockQuack](std::function<void()>) mutable
        {
            ++flyCounter;
            if (flyCounter % 2 == 0)
            {
                mockQuack(); 
            }
        };
    auto mockDance = []() {};

    ModelDuck duck;

    duck.SetFlyBehavior(mockFly);
    duck.SetDanceBehavior(mockDance);
    duck.SetQuackBehavior(mockQuack);


    ASSERT_EQ(quackCounter, 0);
    ASSERT_EQ(flyCounter, 0);

    duck.Fly(); 
    ASSERT_EQ(quackCounter, 0);
    ASSERT_EQ(flyCounter, 1);

    duck.Fly();
    ASSERT_EQ(quackCounter, 1);
    ASSERT_EQ(flyCounter, 2);
}