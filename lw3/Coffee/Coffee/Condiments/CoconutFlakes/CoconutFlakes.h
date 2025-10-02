#pragma once
#include "../CondimentDecorator.h"

class CoconutFlakes : public CondimentDecorator
{
public:
    CoconutFlakes(IBeveragePtr&& beverage, unsigned mass);

protected:
    std::string GetCondimentDescription() const override;
    double GetCondimentCost() const override;

private:
    unsigned m_mass;
};