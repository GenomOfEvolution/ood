#pragma once
template <typename T>
class IObservable;

template <typename T>
class IObserver
{
public:
	virtual void Update(IObservable<T>& observable, T const& data) = 0;
	virtual ~IObserver() = default;
};