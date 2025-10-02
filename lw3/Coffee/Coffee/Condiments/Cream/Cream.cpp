#include "Cream.h"

Cream::Cream(IBeveragePtr&& beverage)
    : CondimentDecorator(std::move(beverage))
{
}

std::string Cream::GetCondimentDescription() const
{
    return "Cream";
}

double Cream::GetCondimentCost() const
{
    return 25;
}
