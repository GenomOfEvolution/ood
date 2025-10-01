#pragma once
#include "IObservable.h"
#include "IObserver.h"
#include <set>

template <typename T>
class CObservable : public IObservable<T>
{
public:
    using ObserverType = IObserver<T>;

    void RegisterObserver(ObserverType& observer) override
    {
        m_observers.emplace(&observer);
    }

    void NotifyObservers() override
    {
        auto data = GetChangedData();
        auto observersCopy = m_observers;

        for (auto& observer : observersCopy)
        {
            observer->Update(*this, data);
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