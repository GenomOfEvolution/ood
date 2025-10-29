#pragma once
#include "../IOutlineStyle.h"

class ShapeLineStyle : public IOutlineStyle
{
public:
	std::optional<double> GetThickness() const override { return m_thickness; };
	void SetThickness(double thickness) override { m_thickness = thickness; };

private:
	double m_thickness = 1;
};