#pragma once
#include "PriorityObservable.h"
#include "WeatherData.h"

struct OutsideWeatherInfo : WeatherInfo
{
	double windSpeed = 0;
	double windDirection = 0;
};

class OutsideWeatherData : public PriorityObservable<OutsideWeatherInfo>
{
public:
	double GetTemperature() const {	return m_temperature; }
	double GetHumidity() const { return m_humidity; }
	double GetPressure() const { return m_pressure;	}
	double GetWindSpeed() const	{ return m_windSpeed; }
	double GetWindDirection() const	{ return m_windDirection; }

	void MeasurementsChanged() { NotifyObservers(); }

	void SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;
		m_windSpeed = windSpeed;
		m_windDirection = windDirection;

		MeasurementsChanged();
	}

protected:
	OutsideWeatherInfo GetChangedData() const override
	{
		OutsideWeatherInfo info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		info.windSpeed = GetWindSpeed();
		info.windDirection = GetWindDirection();
		return info;
	}

private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
	double m_windSpeed = 0;
	double m_windDirection = 0;
};