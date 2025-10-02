#include "Coffee.h"

Coffee::Coffee(const std::string& description)
	: Beverage(description)
{
}

double Coffee::GetCost() const
{
	return 60;
}
