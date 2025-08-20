#pragma once
#include <functional>

struct IFlyBehavior
{
	virtual ~IFlyBehavior() {};
	virtual void Fly(std::function<void()> onFlightEvent = nullptr) = 0;
};
