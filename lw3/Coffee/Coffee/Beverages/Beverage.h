#pragma once
#include "IBeverage.h"

class Beverage : public IBeverage
{
public:
	Beverage(const std::string& description);
	std::string GetDescription() const override;
	
private:
	std::string m_description;
};
