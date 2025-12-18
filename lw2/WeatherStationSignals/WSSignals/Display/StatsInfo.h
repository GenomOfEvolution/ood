#pragma once
#include <cmath>
#include <ostream>

class StatsInfo
{
public:
	explicit StatsInfo(const std::string& name)
		: m_name(name)
	{	}

	void Update(double value)
	{
		m_minValue = std::min(m_minValue, value);
		m_maxValue = std::max(m_maxValue, value);
		m_accValue += value;
		++m_countAcc;
	}

	void Print(std::ostream& out) const
	{
		out << "Max " << m_name << " " << m_maxValue << std::endl;
		out << "Min " << m_name << " " << m_minValue << std::endl;
		out << "Average " << m_name << " " << (m_accValue / m_countAcc) << std::endl;
		out << "---------------------\n";
	}

private:
	std::string m_name;
	double m_minValue = std::numeric_limits<double>::infinity();
	double m_maxValue = -std::numeric_limits<double>::infinity();
	double m_accValue = 0;
	unsigned m_countAcc = 0;
};