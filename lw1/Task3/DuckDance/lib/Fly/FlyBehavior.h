#pragma once
#include <iostream>

auto FlyNoWay = [](std::function<void()> onFlightEvent)
{

};


auto CreateFlyWithWings() 
{
    int flyCount = 0;
    return [flyCount](std::function<void()> onFlightEvent) mutable 
    {
        flyCount++;
        std::cout << "I'm flying with wings!! Flight #" << flyCount << std::endl;

        if (onFlightEvent && flyCount % 2 == 0) 
        {
            onFlightEvent();
        }
    };
}