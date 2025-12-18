#pragma once

template <typename TData, typename TEvent>
class IObserver
{
public:
	virtual void Update(TData const& data, TEvent const& eventType) = 0;
	virtual ~IObserver() = default;
};