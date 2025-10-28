#pragma once
#include "ICommand.h"

class AbstractUndoableCommand : public ICommand
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

	void Unexecute() override 
	{
		if (m_executed)
		{
			DoUnexecute();
			m_executed = false;
		}
	}

	bool ShouldSaveToHistory() const override
	{
		return true;
	}

protected:
	virtual void DoExecute() = 0;
	virtual void DoUnexecute() = 0;

private:
	bool m_executed = false;
};