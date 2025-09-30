#pragma once
#include "IObservable.h"
#include "IObserver.h"

#include <set>
#include <functional>

template <class T>
class CObservable : public IObservable<T>
{
public:
	using ObserverType = IObserver<T>;

	void RegisterObserver(ObserverType& observer) override
	{
		m_observers.insert(&observer);
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();
		for (auto& observer : m_observers)
		{
			observer->Update(data);
		}
	}

	void RemoveObserver(ObserverType& observer) override
	{
		m_observers.erase(&observer);
	}

protected:
	virtual T GetChangedData() const = 0;

private:
	std::set<ObserverType*> m_observers;
};