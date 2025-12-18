#pragma once
#include "../Observer/IObserver.h"
#include "../Observable/WeatherData.h"
#include "../Observable/OutsideWeatherData.h"
#include "StatsInfo.h"
#include "WindDirectionInfo.h"

#include <iostream>

class StatsDisplay final 
	: public IObserver<WeatherInfo>
	, public IObserver<OutsideWeatherInfo>
{
public:
	explicit StatsDisplay(
		std::ostream& output,
		int priority,
		IObservable<WeatherInfo>& indoorObserver,
		IObservable<OutsideWeatherInfo>& outdoorObserver)
		: m_output(output)
		, m_indoorObservable(indoorObserver)
		, m_outdoorObservable(outdoorObserver)
	{
		m_indoorObservable.RegisterObserver(*this, priority);
		m_outdoorObservable.RegisterObserver(*this, priority);
	}

	~StatsDisplay() override
	{
		m_indoorObservable.RemoveObserver(*this);
		m_outdoorObservable.RemoveObserver(*this);
	}

private:
	void Update(WeatherInfo const& data) override
	{
		m_temperatureInfo.Update(data.temperature);
		m_humidityInfo.Update(data.humidity);
		m_pressureInfo.Update(data.pressure);

		m_output << "-------Indoor--------\n";
		m_temperatureInfo.Print(std::cout);
		m_humidityInfo.Print(std::cout);
		m_pressureInfo.Print(std::cout);
		m_windSpeedInfo.Print(std::cout);
		m_windDirectionInfo.Print(std::cout);
	}

	void Update(const OutsideWeatherInfo& data) override
	{
		m_temperatureInfo.Update(data.temperature);
		m_humidityInfo.Update(data.humidity);
		m_pressureInfo.Update(data.pressure);
		m_windSpeedInfo.Update(data.windSpeed);
		m_windDirectionInfo.Update(data.windDirection);

		m_output << "-------Outdoor-------\n";
		m_temperatureInfo.Print(std::cout);
		m_humidityInfo.Print(std::cout);
		m_pressureInfo.Print(std::cout);
		m_windSpeedInfo.Print(std::cout);
		m_windDirectionInfo.Print(std::cout);
	}

	std::ostream& m_output;
	IObservable<WeatherInfo>& m_indoorObservable;
	IObservable<OutsideWeatherInfo>& m_outdoorObservable;

	StatsInfo m_temperatureInfo{ "Temperature" };
	StatsInfo m_humidityInfo{ "Humidity" };
	StatsInfo m_pressureInfo{ "Pressure" };
	StatsInfo m_windSpeedInfo{ "Wind speed" };
	WindDirectionInfo m_windDirectionInfo;
};