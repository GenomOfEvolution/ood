#include "Tea.h"

std::string TeaTypeToString(const TeaType& type)
{
	switch (type)
	{
	case TeaType::Black:
		return "Black";
	case TeaType::White:
		return "White";
	case TeaType::Red:
		return "Red";
	case TeaType::Green:
		return "Green";
	default:
		return "Black";
	}
}

Tea::Tea(TeaType type)
	: Beverage("Tea")
	, m_type(type)
{
}

double Tea::GetCost() const
{
	return 30;
}

std::string Tea::GetDescription() const
{
	return TeaTypeToString(m_type) + " tea";
}
