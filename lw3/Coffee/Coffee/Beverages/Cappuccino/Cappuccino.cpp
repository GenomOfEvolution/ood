#include "Cappuccino.h"

Cappuccino::Cappuccino(CoffeeSize portion)
	: Coffee("Cappuccino")
	, m_portion(portion)
{
}

double Cappuccino::GetCost() const
{
	// TODO: изменить цену
	return 80;
}

std::string Cappuccino::GetDescription() const
{
	return std::string(m_portion == CoffeeSize::Standard ? "Standard" : "Double") + " Cappuccino";
}
