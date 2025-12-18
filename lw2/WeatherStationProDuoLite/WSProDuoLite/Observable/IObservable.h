#pragma once
#include "../Observer/IObserver.h"

template <typename TData, typename TEvent>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(IObserver<TData, TEvent>& observer, int priority, TEvent eventType) = 0;
	virtual void RemoveObserver(IObserver<TData, TEvent>& observer, TEvent eventType) = 0;
};