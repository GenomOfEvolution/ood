#pragma once
#include "../Observer/IObserver.h"
#include "../Observable/WeatherData.h"
#include "../Observable/OutsideWeatherData.h"

#include <ostream>

class Display final 
	: public IObserver<WeatherInfo>
	, public IObserver<OutsideWeatherInfo>
{
public:
	explicit Display(
		std::ostream& output,
		int priority,
		IObservable<WeatherInfo>& indoorObserver,
		IObservable<OutsideWeatherInfo>& outdoorObserver)
		: m_output(output)
		, m_indoorObservable(indoorObserver)
		, m_outdoorObservable(outdoorObserver)
	{
		m_indoorObservable.RegisterObserver(*this, priority);
		m_outdoorObservable.RegisterObserver(*this, priority);
	}

	~Display() override
	{
		m_indoorObservable.RemoveObserver(*this);
		m_outdoorObservable.RemoveObserver(*this);
	}

private:
	void Update(WeatherInfo const& data) override
	{
		m_output << "-------Indoor--------\n";
		PrintInfo(data);
	}

	void Update(const OutsideWeatherInfo& data) override
	{
		m_output << "-------Outdoor-------\n";
		PrintInfo(data);
	}

	void PrintInfo(const WeatherInfo& data) const
	{
		m_output << "Current Temp " << data.temperature << std::endl;
		m_output << "Current Hum " << data.humidity << std::endl;
		m_output << "Current Pressure " << data.pressure << std::endl;
		m_output << "---------------------\n";
	}

	std::ostream& m_output;
	IObservable<WeatherInfo>& m_indoorObservable;
	IObservable<OutsideWeatherInfo>& m_outdoorObservable;
};
