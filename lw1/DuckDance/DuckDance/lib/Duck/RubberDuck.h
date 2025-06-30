#pragma once
#include "Duck.h"
#include "../Fly/FlyNoWay.h"
#include "../Quack/SqueakBehavior.h"
#include "../Dance/CantDance.h"

class RubberDuck : public Duck
{
public:
	RubberDuck()
		: Duck(
			std::make_unique<FlyNoWay>(),
			std::make_unique<SqueakBehavior>(),
			std::make_unique<CantDance>())
	{
	}

	void Display() const override
	{
		std::cout << "I'm rubber duck" << std::endl;
	}
};