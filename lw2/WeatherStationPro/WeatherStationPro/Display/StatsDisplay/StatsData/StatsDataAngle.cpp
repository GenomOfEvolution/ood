#define _USE_MATH_DEFINES
#include "StatsDataAngle.h"
#include <cmath>

double DegreesToRadians(double val)
{
	return val * M_PI / 180;
}

double RadiansToDegrees(double val)
{
	return val * 180 / M_PI;
}

void StatsDataAngle::Update(double value)
{
	const auto angle = DegreesToRadians(value);
	m_sinSum += std::sin(angle);
	m_cosSum += std::cos(angle);
}

void StatsDataAngle::Print(const std::string& statsName, std::ostream& output) const
{
	output << "Average " + statsName + " " << RadiansToDegrees(std::atan2(m_sinSum, m_cosSum)) << std::endl;
	output << "----------------" << std::endl;
}

StatsAngle StatsDataAngle::GetStats() const
{
	return {
		.average = RadiansToDegrees(std::atan2(m_sinSum, m_cosSum)),
	};
}

