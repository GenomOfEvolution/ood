#pragma once
#include "../CondimentDecorator.h"

class ChocolateCrumbs : public CondimentDecorator
{
public:
	ChocolateCrumbs(IBeveragePtr&& beverage, unsigned mass);

protected:
	std::string GetCondimentDescription() const override;
	double GetCondimentCost() const override;

private:
    unsigned m_mass;
};