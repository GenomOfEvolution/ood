#pragma once
#include "Duck.h"
#include "../Fly/FlyBehavior.h"
#include "../Dance/DanceBehavior.h"
#include "../Quack/QuakBehavior.h"

class DecoyDuck : public Duck
{
public:
	DecoyDuck()
		: Duck(FlyNoWay,
			MuteQuackBehavior,
			CantDance)
	{
	}

	void Display() const override
	{
		std::cout << "I'm decoy duck" << std::endl;
	}
};