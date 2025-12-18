#pragma once
#include "Observable/WeatherData.h"
#include "Observer/IObserver.h"

class CountingObserver : public IObserver<WeatherInfo, WeatherType>
{
public:
	int GetCallCount() const { return m_count; }
	WeatherType GetLastEvent() const { return m_lastEvent; }

private:
	void Update(const WeatherInfo& data, const WeatherType& eventType) override
	{
		++m_count;
		m_lastEvent = eventType;
	}

	int m_count = 0;
	WeatherType m_lastEvent{};
};
