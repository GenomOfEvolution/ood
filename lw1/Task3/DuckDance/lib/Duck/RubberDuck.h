#pragma once
#include "Duck.h"
#include "../Fly/FlyBehavior.h"
#include "../Quack/QuakBehavior.h"
#include "../Dance/DanceBehavior.h"

class RubberDuck : public Duck
{
public:
	RubberDuck()
		: Duck(FlyNoWay,
			SqueakBehavior,
			CantDance)
	{
	}

	void Display() const override
	{
		std::cout << "I'm rubber duck" << std::endl;
	}
};