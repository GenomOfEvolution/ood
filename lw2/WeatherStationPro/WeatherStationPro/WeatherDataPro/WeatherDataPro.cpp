#include "WeatherDataPro.h"

double CWeatherDataPro::GetTemperature() const
{
	return m_temperature;
}

double CWeatherDataPro::GetHumidity() const
{
	return m_humidity;
}

double CWeatherDataPro::GetPressure() const
{
	return m_pressure;
}

double CWeatherDataPro::GetWindSpeed() const
{
	return m_windSpeed;
}

double CWeatherDataPro::GetWindDirection() const
{
	return m_windDirection;
}

void CWeatherDataPro::MeasurementsChanged()
{
	NotifyObservers();
}

void CWeatherDataPro::SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection)
{
	m_humidity = humidity;
	m_temperature = temp;
	m_pressure = pressure;
	m_windDirection = windDirection;
	m_windSpeed = windSpeed;

	MeasurementsChanged();
}

SWeatherInfo CWeatherDataPro::GetChangedData() const
{
	SWeatherInfo info;
	info.temperature = GetTemperature();
	info.humidity = GetHumidity();
	info.pressure = GetPressure();
	info.windSpeed = GetWindSpeed();
	info.windDirection = GetWindDirection();

	return info;
}