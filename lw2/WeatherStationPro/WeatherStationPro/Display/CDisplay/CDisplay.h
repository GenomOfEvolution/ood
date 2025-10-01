#pragma once
#include "../../Observer/IObserver.h"
#include "../../WeatherDataPro/WeatherDataPro.h"

class CDisplay : public IObserver<SWeatherInfo>
{
private:
	void Update(SWeatherInfo const& data) override;
};