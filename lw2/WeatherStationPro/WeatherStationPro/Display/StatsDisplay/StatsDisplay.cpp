#include "StatsDisplay.h"
#include <iostream>

void CStatsDisplay::Update(SWeatherInfo const& data)
{
	m_temperature.Update(data.temperature);
	m_humidity.Update(data.humidity);
	m_pressure.Update(data.pressure);

	m_temperature.Print("Temp", std::cout);
	m_humidity.Print("Humid", std::cout);
	m_pressure.Print("Pressure", std::cout);

	std::cout << "\n";
}