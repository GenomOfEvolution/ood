#pragma once
#include "../CondimentDecorator.h"

enum class IceCubeType
{
	Dry,	// Сухой лед (для суровых сибирских мужиков)
	Water	// Обычные кубики из воды
};

class IceCubes : public CondimentDecorator
{
public:
	IceCubes(IBeveragePtr&& beverage, unsigned quantity, IceCubeType type = IceCubeType::Water);

protected:
	double GetCondimentCost() const override;
	std::string GetCondimentDescription() const override;
	
private:
	unsigned m_quantity;
	IceCubeType m_type;
};