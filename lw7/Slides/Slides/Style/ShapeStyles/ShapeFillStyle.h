#pragma once
#include "../IStyle.h"

class ShapeFillStyle : public IStyle
{
public:
	std::optional<bool> IsEnabled() const override { return m_isEnabled; };
	void Enable() override { m_isEnabled = true; };
	void Disable() override { m_isEnabled = false; };

	std::optional<RGBAColor> GetColor() const override { return m_color; };
	void SetColor(RGBAColor color) override { m_color = color; };

private:
	bool m_isEnabled = true;
	RGBAColor m_color = 0x00000000;
};