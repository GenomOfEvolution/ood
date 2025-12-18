#pragma once
#include <numbers>
#include <cmath>
#include <ostream>

class WindDirectionInfo
{
public:
	void Update(double direction)
	{
		double radians = DegreesToRadians(direction);

		double x = std::cos(radians);
		double y = std::sin(radians);

		m_sumX += x;
		m_sumY += y;
		++m_count;
	}

	void Print(std::ostream& out) const
	{
		double radians = std::atan2(m_sumY, m_sumX);
		double degrees = RadiansToDegrees(radians);
		degrees = NormalizeAngle(degrees);

		out << "Average wind direction " << degrees << std::endl;
	}

private:
	static double DegreesToRadians(double degrees)
	{
		return degrees * std::numbers::pi / 180.0;
	}

	static double RadiansToDegrees(double radians)
	{
		return radians * 180.0 / std::numbers::pi;
	}

	static double NormalizeAngle(double angle)
	{
		double normalized = std::fmod(angle, 360.0);
		if (normalized < 0.0)
			normalized += 360.0;

		return normalized;
	}

	double m_sumX = 0.0; 
	double m_sumY = 0.0;
	unsigned int m_count = 0;
};