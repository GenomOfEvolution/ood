#pragma once
#include "../CondimentDecorator.h"

enum class LiqueurType
{
	Nut,
	Chocolate,
};

class Liqueur : public CondimentDecorator
{
public:
	Liqueur(IBeveragePtr&& beverage, LiqueurType type = LiqueurType::Nut);

protected:
	std::string GetCondimentDescription() const override;
	double GetCondimentCost() const override;

private:
	LiqueurType m_type;
};