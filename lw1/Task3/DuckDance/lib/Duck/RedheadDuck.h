#pragma once
#include "Duck.h"
#include "../Fly/FlyBehavior.h"
#include "../Quack/QuakBehavior.h"
#include "../Dance/DanceBehavior.h"

class RedheadDuck : public Duck
{
public:
	RedheadDuck()
		: Duck(CreateFlyWithWings(),
			QuackBehavior,
			MinuetDance)
	{
	}

	void Display() const override
	{
		std::cout << "I'm redhead duck" << std::endl;
	}
};