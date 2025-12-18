#pragma once
#include "../Observable/PriorityObservable.h"
#include "../Observer/IObserver.h"

#include <string>
#include <iostream>

struct CatInfo
{
	std::string location;
	bool isSleeping = false;
};

enum class CatEvent
{
	SaidMeow,
	LocationChanged,
	SleepStateChanged
};

class Cat : public PriorityObservable<CatInfo, CatEvent>
{
public:
	void MoveTo(const std::string& location)
	{
		if (!m_isSleeping)
		{
			m_location = location;
			NotifyObservers(CatEvent::LocationChanged);
		}
	}

	void WakeUp()
	{
		m_isSleeping = true;
		NotifyObservers(CatEvent::SleepStateChanged);
	}

	void GoSleep()
	{
		m_isSleeping = false;
		NotifyObservers(CatEvent::SleepStateChanged);
	}

	void SayMeow(const std::string& message)
	{
		std::cout << "[Cat] said: " << message << ", from " << m_location << std::endl;
		NotifyObservers(CatEvent::SaidMeow);
	}

protected:
	CatInfo GetChangedData() const override
	{
		return {
			.location = m_location,
			.isSleeping = m_isSleeping
		};
	}

private:
	std::string m_location;
	bool m_isSleeping = false;
};

class Mouse : public IObserver<CatInfo, CatEvent>
{
public:
	explicit Mouse(const std::string& name, std::ostream& output)
		: m_name(name)
		, m_output(output)
	{	}

private:
	void Update(const CatInfo& data, const CatEvent& eventType) override
	{
		switch (eventType)
		{
		case CatEvent::SaidMeow:
			m_output << "[Mouse " << m_name << "] Squeeeck!" << std::endl;
			break;
		case CatEvent::SleepStateChanged:
			if (data.isSleeping)
			{
				m_output << "[Mouse " << m_name << "] runs away!" << std::endl;
			}
			else
			{
				m_output << "[Mouse " << m_name << "] hides in a hole!" << std::endl;
			}
		}
	}

	std::string m_name;
	std::ostream& m_output;
};

class Dog : public IObserver<CatInfo, CatEvent>
{
public:
	explicit Dog(const std::string& name, std::ostream& output)
		: m_name(name)
		, m_output(output)
	{
	}

private:
	void Update(const CatInfo& data, const CatEvent& eventType) override
	{
		switch (eventType)
		{
		case CatEvent::SaidMeow:
			m_output << "[Dog " << m_name << "] Bark-Bark!" << std::endl;
			break;
		case CatEvent::LocationChanged:
			m_output << "[Dog " << m_name << "] where this cat goes? (" << data.location << ")" << std::endl;
			break;
		}
	}

	std::string m_name;
	std::ostream& m_output;
};