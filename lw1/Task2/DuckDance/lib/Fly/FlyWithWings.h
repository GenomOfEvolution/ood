#pragma once
#include "IFlyBehavior.h"
#include <iostream>

class FlyWithWings : public IFlyBehavior
{
private:
	int m_flyCount = 0;

public:
	void Fly(std::function<void()> onFlightEvent = nullptr) override
	{
		++m_flyCount;
		std::cout << "I'm flying with wings!!" 
			<< " Flight #" << m_flyCount << std::endl;

		if (onFlightEvent && m_flyCount % 2 == 0)
		{
			onFlightEvent();
		}
	}
};