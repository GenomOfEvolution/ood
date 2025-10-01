#pragma once
#include "IObservable.h"
#include "IObserver.h"

#include <map>
#include <unordered_map>
#include <stdexcept>

template <class T>
class CObservable : public IObservable<T>
{
public:
	using ObserverType = IObserver<T>;

	void RegisterObserver(ObserverType& observer, int priority) override
	{
		if (m_observerItMap.find(&observer) == m_observerItMap.end())
		{
			auto it = m_observers.emplace(priority, &observer);
			m_observerItMap[&observer] = it;
		}
		else
		{
			throw std::runtime_error("Observer is already registered");
		}
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();
		auto observers = m_observers;

		for (auto& observer : observers)
		{
			observer.second->Update(data);
		}
	}

	void RemoveObserver(ObserverType& observer) override
	{
		auto it = m_observerItMap.find(&observer);
		if (it != m_observerItMap.end())
		{
			m_observers.erase(it->second);
			m_observerItMap.erase(it);
		}
	}

protected:
	virtual T GetChangedData() const = 0;

private:
	std::multimap<int, ObserverType*> m_observers;
	std::unordered_map<ObserverType*, typename std::multimap<int, ObserverType*>::iterator> m_observerItMap;
};