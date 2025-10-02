#include "IceCubes.h"

IceCubes::IceCubes(IBeveragePtr&& beverage, unsigned quantity, IceCubeType type)
	: CondimentDecorator(move(beverage))
	, m_quantity(quantity)
	, m_type(type)
{	}

double IceCubes::GetCondimentCost() const
{
	return (m_type == IceCubeType::Dry ? 10 : 5) * m_quantity;
}

std::string IceCubes::GetCondimentDescription() const
{
	return std::string(m_type == IceCubeType::Dry ? "Dry" : "Water")
		+ " ice cubes x " + std::to_string(m_quantity);
}
