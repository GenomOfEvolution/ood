#pragma once
#include "Observer/IObserver.h"
#include "Observable/WeatherData.h"
#include <vector>

class CallOrderDisplay : public IObserver<WeatherInfo>
{
public:
	CallOrderDisplay(std::vector<int>& callOrder, int id)
		: m_callOrder(callOrder)
		, m_id(id)
	{	}

	void Update(const WeatherInfo& data) override
	{
		m_callOrder.push_back(m_id);
	}

private:
	std::vector<int>& m_callOrder;
	int m_id;
};