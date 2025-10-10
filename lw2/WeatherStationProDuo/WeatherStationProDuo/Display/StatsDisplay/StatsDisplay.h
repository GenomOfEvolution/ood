#pragma once
#include "../../Observer/IObserver.h"
#include "../../WeatherData/WeatherData.h"
#include "StatsData/StatsData.h"
#include <map>

class CStatsDisplay : public IObserver<SWeatherInfo>
{
public:
	void RegisterObservable(IObservable<SWeatherInfo>& observable, const std::string& name);
	void RemoveObservable(IObservable<SWeatherInfo>& observable);

private:
	void Update(IObservable<SWeatherInfo>& observable, SWeatherInfo const& data) override;
	void PrintObservableName(IObservable<SWeatherInfo>& observable);

	struct ObservableData
	{
		std::string name;

		StatsData temperature;
		StatsData humidity;
		StatsData pressure;
	};

	std::map<IObservable<SWeatherInfo>*, ObservableData> m_observables;
};