#pragma once
#include "../../Observer/IObserver.h"
#include "../../WeatherDataPro/WeatherDataPro.h"
#include "StatsData/StatsData.h"

class CStatsDisplay : public IObserver<SWeatherInfo>
{
private:
	void Update(SWeatherInfo const& data) override;

	StatsData m_temperature;
	StatsData m_humidity;
	StatsData m_pressure;
};