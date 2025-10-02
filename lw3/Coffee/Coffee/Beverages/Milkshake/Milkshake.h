#pragma once
#include "../Beverage.h"

enum class MilkshakeSize
{
	Small,
	Medium,
	Big,
};

class Milkshake : public Beverage
{
public:
	Milkshake(MilkshakeSize size = MilkshakeSize::Medium);
	double GetCost() const override;
	std::string GetDescription() const override;

private:
	MilkshakeSize m_size;
};
