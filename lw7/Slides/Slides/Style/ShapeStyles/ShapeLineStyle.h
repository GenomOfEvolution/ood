#pragma once
#include "../IOutlineStyle.h"

class ShapeLineStyle : public IOutlineStyle
{
public:
	std::optional<bool> IsEnabled() const override { return m_isEnabled; };
	void Enable() override { m_isEnabled = true; };
	void Disable() override { m_isEnabled = false; };

	std::optional<RGBAColor> GetColor() const override { return m_color; };
	void SetColor(RGBAColor color) override { m_color = color; };

	std::optional<double> GetThickness() const override { return m_thickness; };
	void SetThickness(double thickness) override { m_thickness = thickness; };

private:
	bool m_isEnabled = true;
	RGBAColor m_color = 0x00000000;
	double m_thickness = 1;
};