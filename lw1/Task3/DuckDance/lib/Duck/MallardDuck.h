#pragma once
#include "Duck.h"
#include "../Fly/FlyBehavior.h"
#include "../Quack/QuakBehavior.h"
#include "../Dance/DanceBehavior.h"

class MallardDuck : public Duck
{
public:
	MallardDuck()
		: Duck(CreateFlyWithWings(),
			QuackBehavior,
			WaltzDance)
	{
	}

	void Display() const override
	{
		std::cout << "I'm mallard duck" << std::endl;
	}
};
