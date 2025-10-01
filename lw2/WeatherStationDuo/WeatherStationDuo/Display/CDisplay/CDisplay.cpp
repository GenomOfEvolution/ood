#include "CDisplay.h"
#include <iostream>

void CDisplay::RegisterObservable(IObservable<SWeatherInfo>& observable, const std::string& name)
{
	m_observables[&observable] = name;
}

void CDisplay::RemoveObservable(IObservable<SWeatherInfo>& observable)
{
	m_observables.erase(&observable);
}

void CDisplay::Update(IObservable<SWeatherInfo>& observable, SWeatherInfo const& data)
{
	PrintObservableName(observable);

	std::cout << "Current Temp " << data.temperature << std::endl;
	std::cout << "Current Hum " << data.humidity << std::endl;
	std::cout << "Current Pressure " << data.pressure << std::endl;
	std::cout << "----------------" << std::endl;
}

void CDisplay::PrintObservableName(IObservable<SWeatherInfo>& observable)
{
	auto it = m_observables.find(&observable);
	if (it != m_observables.end())
	{
		std::cout << it->second;
	}
	else
	{
		std::cout << "unknown";
	}
	std::cout << " source" << std::endl;
}
