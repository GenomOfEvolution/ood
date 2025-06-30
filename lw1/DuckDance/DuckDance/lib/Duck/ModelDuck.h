#pragma once
#include "Duck.h"
#include "../Fly/FlyNoWay.h"
#include "../Quack/QuackBehavior.h"
#include "../Dance/CantDance.h"

class ModelDuck : public Duck
{
public:
	ModelDuck()
		: Duck(
			std::make_unique<FlyNoWay>(),
			std::make_unique<QuackBehavior>(),
			std::make_unique<CantDance>())
	{
	}

	void Display() const override
	{
		std::cout << "I'm model duck" << std::endl;
	}
};