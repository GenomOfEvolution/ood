#pragma once
#include <limits>
#include <iostream>

struct StatsAngle
{
	double average = 0;
};

class StatsDataAngle
{
public:
	void Update(double value);
	void Print(const std::string& statsName, std::ostream& output) const;
	StatsAngle GetStats() const;

private:
	double m_sinSum = 0;
	double m_cosSum = 0;
};