#include "ChocolateCrumbs.h"

ChocolateCrumbs::ChocolateCrumbs(IBeveragePtr&& beverage, unsigned mass)
    : CondimentDecorator(std::move(beverage))
    , m_mass(mass)
{
}

std::string ChocolateCrumbs::GetCondimentDescription() const
{
    return "Chocolate crumbs " + std::to_string(m_mass) + "g";
}

double ChocolateCrumbs::GetCondimentCost() const
{
    return 2 * m_mass;
}