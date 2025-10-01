#include "StatsData.h"

void StatsData::Update(double value)
{
	if (m_min > value)
	{
		m_min = value;
	}

	if (m_max < value)
	{
		m_max = value;
	}

	m_accumulated += value;
	++m_countAcc;
}

void StatsData::Print(const std::string& statsName, std::ostream& output) const
{
	output << "Max " + statsName + " " << m_max << std::endl;
	output << "Min " + statsName + " " << m_min << std::endl;
	output << "Average " + statsName + " " << (m_accumulated / m_countAcc) << std::endl;
	output << "----------------" << std::endl;
}

Stats StatsData::GetStats() const
{
	return {
		.max = m_max,
		.min = m_min,
		.average = m_accumulated / m_countAcc
	};
}
