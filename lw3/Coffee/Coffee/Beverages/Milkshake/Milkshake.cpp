#include "Milkshake.h"

std::string MilkshakeSizeToString(const MilkshakeSize& size)
{
	switch (size)
	{
	case MilkshakeSize::Small:
		return "Small";
	case MilkshakeSize::Medium:
		return "Standard";
	case MilkshakeSize::Big:
		return "Huge";
	default:
		return "Standard";
	}
}

Milkshake::Milkshake(MilkshakeSize size)
	: Beverage("Milkshake")
	, m_size(size)
{
}

double Milkshake::GetCost() const
{
	switch (m_size)
	{
	case MilkshakeSize::Small:
		return 50;
	case MilkshakeSize::Medium:
		return 60;
	case MilkshakeSize::Big:
		return 80;
	default:
		return 60;
	}
}

std::string Milkshake::GetDescription() const
{
	return MilkshakeSizeToString(m_size) + " milkshake";
}
