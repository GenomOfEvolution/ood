#pragma once
#include "../Beverage.h"

enum class CoffeeSize
{
	Standard,
	Double,
};

class Coffee : public Beverage
{
public:
	Coffee(const std::string& description = "Coffee");
	double GetCost() const override;
};