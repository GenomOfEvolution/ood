#pragma once
#include "../Beverage.h"

enum class TeaType
{
	Black,
	White,
	Red,
	Green
};

class Tea : public Beverage
{
public:
	Tea(TeaType type = TeaType::Black);
	double GetCost() const override;
	std::string GetDescription() const override;

private:
	TeaType m_type;
};