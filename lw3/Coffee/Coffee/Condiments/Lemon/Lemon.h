#pragma once
#include "../CondimentDecorator.h"

class Lemon : public CondimentDecorator
{
public:
	Lemon(IBeveragePtr&& beverage, unsigned quantity = 1);

protected:
	double GetCondimentCost() const override;
	std::string GetCondimentDescription() const override;
	
private:
	unsigned m_quantity;
};