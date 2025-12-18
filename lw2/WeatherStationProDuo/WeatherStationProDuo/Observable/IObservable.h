#pragma once
#include "../Observer/IObserver.h"

template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(IObserver<T>& observer, int priority) = 0;
	virtual void RemoveObserver(IObserver<T>& observer) = 0;
};