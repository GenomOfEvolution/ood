#pragma once
#include "../CondimentDecorator.h"

class ChocolateSlices : public CondimentDecorator
{
public:
	ChocolateSlices(IBeveragePtr&& beverage, unsigned amount);

protected:
	std::string GetCondimentDescription() const override;
	double GetCondimentCost() const override;

private:
	static const int MAX_SLICES = 5;
	unsigned m_amount;
};
