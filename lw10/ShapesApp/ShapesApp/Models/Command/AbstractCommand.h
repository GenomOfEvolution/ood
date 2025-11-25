#pragma once
#include "ICommand.h"

class AbstractCommand : public ICommand
{
public:
	void Execute() override
	{
		if (!m_executed)
		{
			DoExecute();
			m_executed = true;
		}
	}

protected:
	virtual void DoExecute() = 0;

private:
	bool m_executed = false;
};