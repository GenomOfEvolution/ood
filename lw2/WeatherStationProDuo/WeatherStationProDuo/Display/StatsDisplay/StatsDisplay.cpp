#include "StatsDisplay.h"
#include <iostream>

void CStatsDisplay::RegisterObservable(IObservable<SWeatherInfo>& observable, const std::string& name)
{
	m_observables[&observable].name = name;
}

void CStatsDisplay::RemoveObservable(IObservable<SWeatherInfo>& observable)
{
	m_observables.erase(&observable);
}

void CStatsDisplay::Update(IObservable<SWeatherInfo>& observable, SWeatherInfo const& data)
{
	auto& observableData = m_observables[&observable];

	observableData.humidity.Update(data.humidity);
	observableData.temperature.Update(data.temperature);
	observableData.pressure.Update(data.pressure);

	PrintObservableName(observable);
	observableData.humidity.Print("Humid", std::cout);
	observableData.temperature.Print("Temp", std::cout);
	observableData.pressure.Print("Pressure", std::cout);
	std::cout << "----------------" << std::endl;
}

void CStatsDisplay::PrintObservableName(IObservable<SWeatherInfo>& observable)
{
	auto it = m_observables.find(&observable);
	if (it != m_observables.end())
	{
		std::cout << it->second.name;
	}
	else
	{
		std::cout << "unknown";
	}
	std::cout << " source" << std::endl;
}
