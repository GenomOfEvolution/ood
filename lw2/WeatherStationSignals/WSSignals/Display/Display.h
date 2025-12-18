#pragma once
#include "../Observer/IObserver.h"
#include "../Observable/WeatherData.h"

#include <ostream>

class Display
{
public:
	explicit Display(std::ostream& output)
		: m_output(output)
	{	}

	void OnTemperatureChanged(const WeatherInfo& data)
	{
		m_output << "Current Temp: " << data.temperature << std::endl;
		m_output << "----------------" << std::endl;
	}

	void OnPressureChanged(const WeatherInfo& data)
	{
		m_output << "Current Pressure: " << data.pressure << std::endl;
		m_output << "----------------" << std::endl;
	}

	void OnHumidityChanged(const WeatherInfo& data)
	{
		m_output << "Current Hum: " << data.humidity << std::endl;
		m_output << "----------------" << std::endl;
	}

	void OnWindSpeedChanged(const WeatherInfo& data)
	{
		m_output << "Current Wind Speed: " << data.windSpeed << std::endl;
		m_output << "----------------" << std::endl;
	}

	void OnWindDirectionChanged(const WeatherInfo& data)
	{
		m_output << "Current Wind Direction: " << data.windDirection << std::endl;
		m_output << "----------------" << std::endl;
	}

private:
	std::ostream& m_output;
};
