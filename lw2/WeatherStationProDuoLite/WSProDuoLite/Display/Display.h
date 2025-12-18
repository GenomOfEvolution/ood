#pragma once
#include "../Observer/IObserver.h"
#include "../Observable/WeatherData.h"

#include <ostream>

class Display final : public IObserver<WeatherInfo, WeatherType>
{
public:
	explicit Display(std::ostream& output)
		: m_output(output)
	{
	}

private:
	void Update(WeatherInfo const& data, WeatherType const& eventType) override
	{
		switch (eventType)
		{
		case WeatherType::Temperature:
			PrintTemperature(data);
			break;
		case WeatherType::Humidity:
			PrintHumidity(data);
			break;
		case WeatherType::Pressure: 
			PrintPressure(data);
			break;
		case WeatherType::WindSpeed:
			PrintWindSpeed(data);
			break;
		case WeatherType::WindDirection:
			PrintWindDirection(data);
			break;
		default:
			PrintInfo(data);
			break;
		}
		m_output << "---------------------\n";
	}

	void PrintTemperature(WeatherInfo const& data) const
	{
		m_output << "Current Temp " << data.temperature << std::endl;
	}

	void PrintPressure(WeatherInfo const& data) const
	{
		m_output << "Current Pressure " << data.pressure << std::endl;
	}

	void PrintHumidity(WeatherInfo const& data) const
	{
		m_output << "Current Hum " << data.humidity << std::endl;
	}

	void PrintWindSpeed(WeatherInfo const& data) const
	{
		m_output << "Wind speed " << data.windSpeed << std::endl;
	}

	void PrintWindDirection(WeatherInfo const& data) const
	{
		m_output << "Wind direction " << data.windDirection << std::endl;
	}

	void PrintInfo(const WeatherInfo& data) const
	{
		PrintTemperature(data);
		PrintPressure(data);
		PrintHumidity(data);
		PrintWindSpeed(data);
		PrintWindDirection(data);
	}

	std::ostream& m_output;
};
