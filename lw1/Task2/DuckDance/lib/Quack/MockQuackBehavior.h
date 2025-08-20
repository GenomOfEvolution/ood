#pragma once
#include "IQuakBehavior.h"

class MockQuackBehavior : public IQuackBehavior
{
private:
	bool m_wasQuack = false;

public:
	void Quack() override 
	{
		m_wasQuack = true;
	}

	bool WasQuack()
	{
		return m_wasQuack;
	}
};