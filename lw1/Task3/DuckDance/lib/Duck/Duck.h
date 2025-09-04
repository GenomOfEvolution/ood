#pragma once
#include <cassert>
#include <iostream>
#include <functional>

class Duck
{
public:
	using behavior_t = std::function<void()>;
	using FlyBehavior = std::function<void(behavior_t)>;

	Duck(FlyBehavior flyBehavior,
		behavior_t quackBehavior,
		behavior_t danceBehavior)
		: m_flyBehavior(std::move(flyBehavior))
		, m_quackBehavior(std::move(quackBehavior))
		, m_danceBehavior(std::move(danceBehavior))
	{
		assert(m_flyBehavior);
		assert(m_quackBehavior);
		assert(m_danceBehavior);
	}

	void Quack() const
	{
		m_quackBehavior();
	}

	void Swim() const
	{
		std::cout << "I'm swimming" << std::endl;
	}

	void Fly() const
	{
		m_flyBehavior([this]() {Quack(); });
	}

	void Dance() const
	{
		m_danceBehavior();
	}

	void SetFlyBehavior(FlyBehavior flyBehavior)
	{
		assert(flyBehavior);
		m_flyBehavior = std::move(flyBehavior);
	}

	void SetDanceBehavior(behavior_t danceBehavior)
	{
		assert(danceBehavior);
		m_danceBehavior = std::move(danceBehavior);
	}

	virtual void Display() const = 0;
	virtual ~Duck() = default;

private:
	FlyBehavior m_flyBehavior;
	behavior_t m_quackBehavior;
	behavior_t m_danceBehavior;
};