#pragma once
#include "../../Observer/IObserver.h"
#include "../../WeatherData/WeatherData.h"
#include <map>
#include <string>

class CDisplay : public IObserver<SWeatherInfo>
{
public:
	void RegisterObservable(IObservable<SWeatherInfo>& observable, const std::string& name);
	void RemoveObservable(IObservable<SWeatherInfo>& observable);

private:
	void Update(IObservable<SWeatherInfo>& observable, SWeatherInfo const& data) override;
	void PrintObservableName(IObservable<SWeatherInfo>& observable);

	std::map<IObservable<SWeatherInfo>*, std::string> m_observables;
};