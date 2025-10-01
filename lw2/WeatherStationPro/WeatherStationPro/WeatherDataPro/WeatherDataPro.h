#pragma once
#include "../Observer/CObservable.h"

struct SWeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;

	double windSpeed = 0;
	double windDirection = 0;
};

class CWeatherDataPro : public CObservable<SWeatherInfo>
{
public:
	double GetTemperature() const;
	double GetHumidity() const;
	double GetPressure() const;
	double GetWindSpeed() const;
	double GetWindDirection() const;
	
	void MeasurementsChanged();
	void SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection);

protected:
	SWeatherInfo GetChangedData() const override;
	
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;

	double m_windSpeed = 0;
	double m_windDirection = 0;
};