#include "ChocolateSlices.h"
#include <stdexcept>

ChocolateSlices::ChocolateSlices(IBeveragePtr&& beverage, unsigned amount)
	: CondimentDecorator(std::move(beverage))
{
	if (amount > MAX_SLICES)
	{
		throw std::invalid_argument("Too much slices, u can only have " + MAX_SLICES);
	}
	m_amount = amount;
}

std::string ChocolateSlices::GetCondimentDescription() const
{
	return m_amount + " chocolate slices";
}

double ChocolateSlices::GetCondimentCost() const
{
	return 10 * m_amount;
}


