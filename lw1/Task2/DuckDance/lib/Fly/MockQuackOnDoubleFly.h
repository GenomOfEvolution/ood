#include "IFlyBehavior.h"

class MockQuackOnDoubleFly : public IFlyBehavior
{
private:
	int m_flyCount = 0;

public:
	int GetFlightCount()
	{
		return m_flyCount;
	}

	void Fly(std::function<void()> onFlightEvent = nullptr) override
	{
		++m_flyCount;

		if (onFlightEvent && m_flyCount % 2 == 0)
		{
			onFlightEvent();
		}
	}
};