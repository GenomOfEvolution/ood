#pragma once
#include <limits>
#include <iostream>

struct Stats
{
	double max = 0;
	double min = 0;
	double average = 0;
};

class StatsData
{
public:
	void Update(double value);
	void Print(const std::string& statsName, std::ostream& output) const;
	Stats GetStats() const;

private:
	double m_min = std::numeric_limits<double>::infinity();
	double m_max = -std::numeric_limits<double>::infinity();
	double m_accumulated = 0;
	unsigned m_countAcc = 0;
};