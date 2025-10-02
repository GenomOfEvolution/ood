#pragma once
#include "../Coffee/Coffee.h"

class Cappuccino : public Coffee
{
public:
	Cappuccino(CoffeeSize portion);
	double GetCost() const override;
	std::string GetDescription() const override;
private:
	CoffeeSize m_portion;
};