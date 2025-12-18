#pragma once
#include "Observable/IObservable.h"
#include "Observable/OutsideWeatherData.h"

class MockDisplay 
	: public IObserver<WeatherInfo>
	, public IObserver<OutsideWeatherInfo>
{
public:
	explicit MockDisplay(WeatherData& weatherData, OutsideWeatherData& outWeatherData, int priority = 0)
		: m_data(weatherData)
		, m_outData(outWeatherData)
	{
		m_data.RegisterObserver(*this, priority);
		m_outData.RegisterObserver(*this, priority);
	}

	int GetCallCount() const
	{
		return m_count;
	}

private:
	void Update(const WeatherInfo& data) override
	{
		++m_count;
	}

	void Update(const OutsideWeatherInfo& data) override
	{
		++m_count;
	}

private:
	WeatherData& m_data;
	OutsideWeatherData& m_outData;
	int m_count = 0;
};