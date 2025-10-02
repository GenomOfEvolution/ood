#pragma once
#include "../Coffee/Coffee.h"

class Latte : public Coffee
{
public:
	Latte(CoffeeSize portion = CoffeeSize::Standard);
	double GetCost() const override;
	std::string GetDescription() const override;

private:
	CoffeeSize m_portion;
};