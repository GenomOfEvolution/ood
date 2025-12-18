#pragma once
#include "IObservable.h"
#include <map>
#include <unordered_map>

template <class T>
class PriorityObservable : public IObservable<T>
{
public:
	using ObserverType = IObserver<T>;

	void RegisterObserver(ObserverType& observer, int priority) override
	{
		auto it = m_observerPriorities.find(&observer);
		if (it != m_observerPriorities.end())
		{
			return;
		}

		m_observers.emplace(-priority, &observer);
		m_observerPriorities.emplace(&observer, priority);
	}

	void RemoveObserver(ObserverType& observer) override
	{
		auto it = m_observerPriorities.find(&observer);
		if (it == m_observerPriorities.end())
		{
			return;
		}

		int priority = it->second;
		m_observerPriorities.erase(it);

		auto range = m_observers.equal_range(-priority);
		for (auto& iter = range.first; iter != range.second; ++iter)
		{
			if (iter->second == &observer)
			{
				m_observers.erase(iter);
				break;
			}
		}
	}

protected:
	virtual T GetChangedData() const = 0;

	void NotifyObservers()
	{
		T data = GetChangedData();
		std::multimap<int, ObserverType*> observersCopy(m_observers);

		for (auto& [priority, observer] : observersCopy)
		{
			observer->Update(data);
		}
	}

private:
	std::multimap<int, ObserverType*> m_observers;
	std::unordered_map<ObserverType*, int> m_observerPriorities;
};