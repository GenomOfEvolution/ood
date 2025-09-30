#pragma once
#include "../../Observer/IObserver.h"
#include "../../WeatherData/WeatherData.h"

class CDisplay : public IObserver<SWeatherInfo>
{
private:
	void Update(SWeatherInfo const& data) override;
};