#pragma once
#include "../CondimentDecorator.h"

enum class SyrupType
{
	Chocolate,	// Шоколадный
	Maple,		// Кленовый
};

class Syrup : public CondimentDecorator
{
public:
	Syrup(IBeveragePtr&& beverage, SyrupType syrupType);

protected:
	double GetCondimentCost() const override;
	std::string GetCondimentDescription() const override;
	
private:
	SyrupType m_syrupType;
};