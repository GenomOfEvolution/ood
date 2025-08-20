#pragma once
#include "IFlyBehavior.h"

class FlyNoWay : public IFlyBehavior
{
public:
	void Fly(std::function<void()> onFlightEvent = nullptr) override {}
};