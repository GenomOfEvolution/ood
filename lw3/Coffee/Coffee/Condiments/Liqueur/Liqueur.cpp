#include "Liqueur.h"

std::string LiqueurTypeToStr(const LiqueurType& type)
{
	switch (type)
	{
	case LiqueurType::Nut:
		return "Nut";
		break;
	case LiqueurType::Chocolate:
		return "Chocolate";
		break;
	default:
		return "unknown";
	}
}

Liqueur::Liqueur(IBeveragePtr&& beverage, LiqueurType type)
	: CondimentDecorator(std::move(beverage))
	, m_type(type)
{	}

std::string Liqueur::GetCondimentDescription() const
{
	return LiqueurTypeToStr(m_type) + " liqueur";
}

double Liqueur::GetCondimentCost() const
{
	return 50;
}
