#include "CoconutFlakes.h"

CoconutFlakes::CoconutFlakes(IBeveragePtr&& beverage, unsigned mass)
	: CondimentDecorator(std::move(beverage))
	, m_mass(mass)
{
}

std::string CoconutFlakes::GetCondimentDescription() const
{
	return "Coconut flakes " + std::to_string(m_mass) + "g";
}

double CoconutFlakes::GetCondimentCost() const
{
	return 1.0 * m_mass;
}