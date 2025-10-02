#include "Cinnamon.h"

Cinnamon::Cinnamon(IBeveragePtr&& beverage)
	: CondimentDecorator(std::move(beverage))
{
}

double Cinnamon::GetCondimentCost() const
{
	return 20;
}

std::string Cinnamon::GetCondimentDescription() const
{
	return "Cinnamon";
}