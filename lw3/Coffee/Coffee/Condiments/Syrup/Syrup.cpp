#include "Syrup.h"

Syrup::Syrup(IBeveragePtr&& beverage, SyrupType syrupType)
	: CondimentDecorator(std::move(beverage))
	, m_syrupType(syrupType)
{
}

double Syrup::GetCondimentCost() const
{
	return 15;
}

std::string Syrup::GetCondimentDescription() const
{
	return std::string(m_syrupType == SyrupType::Chocolate ? "Chocolate" : "Maple")
		+ " syrup";
}
