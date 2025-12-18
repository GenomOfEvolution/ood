#pragma once
#include "IObservable.h"
#include <map>
#include <unordered_map>
#include <utility> // Для std::pair

template <class TData, class TEvent>
class PriorityObservable : public IObservable<TData, TEvent> {
public:
    using ObserverType = IObserver<TData, TEvent>;
    using ObserverEventKey = std::pair<ObserverType*, TEvent>;
    using ObserverMap = std::multimap<int, ObserverType*>;
    using ObserverIterator = typename ObserverMap::iterator;

    void RegisterObserver(ObserverType& observer, int priority, TEvent eventType) override
    {
        ObserverEventKey key = std::make_pair(&observer, eventType);

        // Сначала проверяем, не зарегистрирован ли уже наблюдатель
        if (m_observerPriorities.find(key) != m_observerPriorities.end())
        {
            return;
        }

        // Emplace возвращает pair<iterator, bool>
        auto insertResult = m_observerPriorities.emplace(key, priority);

        // Так как мы проверили наличие выше, insertResult.second всегда true
        auto priorityIt = insertResult.first; // Теперь priorityIt - это итератор

        try
        {
            auto eventIt = m_observers.find(eventType);
            if (eventIt == m_observers.end())
            {
                ObserverMap newMap;
                newMap.emplace(-priority, &observer); // Используем -priority для обратной сортировки
                m_observers.emplace(eventType, std::move(newMap));
            }
            else
            {
                eventIt->second.emplace(-priority, &observer);
            }
        }
        catch (...)
        {
            // Теперь корректно удаляем по итератору
            m_observerPriorities.erase(priorityIt);
            throw;
        }
    }

    void NotifyObservers(TEvent eventType) // Добавлен override для соответствия интерфейсу
    {
        TData data = GetChangedData();

        auto eventIt = m_observers.find(eventType);
        if (eventIt == m_observers.end())
        {
            return;
        }

        // Создаем копию для безопасной итерации
        ObserverMap observersCopy = eventIt->second;

        for (const auto& [negPriority, observer] : observersCopy)
        {
            // Проверяем, существует ли наблюдатель в оригинальном контейнере
            auto range = eventIt->second.equal_range(negPriority);
            auto it = std::find_if(range.first, range.second,
                [observer](const auto& item) { return item.second == observer; });

            if (it != range.second)
            {
                observer->Update(data, eventType);
            }
        }
    }

    void RemoveObserver(ObserverType& observer, TEvent eventType) override
    {
        ObserverEventKey key = std::make_pair(&observer, eventType);
        auto priorityIt = m_observerPriorities.find(key);
        if (priorityIt == m_observerPriorities.end())
        {
            return;
        }

        const int priority = priorityIt->second;
        const int negPriority = -priority;

        m_observerPriorities.erase(priorityIt);

        auto eventIt = m_observers.find(eventType);
        if (eventIt == m_observers.end())
        {
            return;
        }

        auto& eventMap = eventIt->second;
        auto range = eventMap.equal_range(negPriority);

        auto it = std::find_if(range.first, range.second,
            [&observer](const auto& item) { return item.second == &observer; });

        if (it == range.second)
        {
            return;
        }

        eventMap.erase(it);

        if (eventMap.empty())
        {
            m_observers.erase(eventIt);
        }
    }

protected:
    virtual TData GetChangedData() const = 0;

private:
    struct ObserverEventKeyHash
    {
        std::size_t operator()(const ObserverEventKey& key) const
        {
            auto h1 = std::hash<ObserverType*>{}(key.first);
            auto h2 = std::hash<TEvent>{}(key.second);
            return h1 ^ (h2 << 1);
        }
    };

    struct ObserverEventKeyEqual {
        bool operator()(const ObserverEventKey& lhs, const ObserverEventKey& rhs) const {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };

    // Добавлен компаратор равенства для unordered_map
    std::unordered_map<TEvent, ObserverMap> m_observers;
    std::unordered_map<ObserverEventKey, int, ObserverEventKeyHash, ObserverEventKeyEqual> m_observerPriorities;
};