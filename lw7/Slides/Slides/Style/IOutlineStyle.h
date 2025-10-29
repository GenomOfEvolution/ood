#pragma once
#include "IStyle.h"

class IOutlineStyle : public IStyle
{
public:
	virtual std::optional<double> GetThickness() const = 0;
	virtual void SetThickness(double thickness) = 0;

	virtual ~IOutlineStyle() = default;
};
