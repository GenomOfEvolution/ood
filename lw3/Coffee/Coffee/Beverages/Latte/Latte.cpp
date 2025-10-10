#include "Latte.h"

Latte::Latte(CoffeeSize portion)
	: Coffee("Latte")
	, m_portion(portion)
{
}

double Latte::GetCost() const
{
	// TODO: изменить цену
	return 90;
}

std::string Latte::GetDescription() const
{
	return std::string(m_portion == CoffeeSize::Standard ? "Standard" : "Double") + " latte";
}
