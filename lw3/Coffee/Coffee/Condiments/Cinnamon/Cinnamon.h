#pragma once
#include "../CondimentDecorator.h"

class Cinnamon : public CondimentDecorator
{
public:
	Cinnamon(IBeveragePtr&& beverage);

protected:
	double GetCondimentCost() const override;
	std::string GetCondimentDescription() const override;
};