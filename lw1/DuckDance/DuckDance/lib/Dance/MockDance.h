#pragma once
#include "IDanceBehavior.h"
#include <iostream>

class MockDance : public IDanceBehavior
{
private:
	bool m_danced = false;

public:
	bool GetDanceStatus()
	{
		return m_danced;
	}

	void Dance() override
	{
		m_danced = true;
	}
};