#pragma once
#include "Observable/WeatherData.h"
#include "Observer/IObserver.h"
#include <vector>

class OrderTrackingObserver : public IObserver<WeatherInfo, WeatherType>
{
public:
	OrderTrackingObserver(std::vector<int>& order, int id)
		: m_order(order), m_id(id) 
	{	}

private:
	void Update(const WeatherInfo& data, const WeatherType& eventType) override
	{
		m_order.push_back(m_id);
	}

	std::vector<int>& m_order;
	int m_id;
};
