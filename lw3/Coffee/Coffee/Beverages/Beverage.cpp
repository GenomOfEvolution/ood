#include "Beverage.h"

Beverage::Beverage(const std::string& description)
	:m_description(description)
{
}

std::string Beverage::GetDescription() const
{
	return m_description;
}
