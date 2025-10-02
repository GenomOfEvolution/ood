#pragma once
#include "../CondimentDecorator.h"

class Cream : public CondimentDecorator
{
public:
    Cream(IBeveragePtr&& beverage);

protected:
    std::string GetCondimentDescription() const override;
    double GetCondimentCost() const override;
};