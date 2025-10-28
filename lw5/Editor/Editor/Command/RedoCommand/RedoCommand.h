#pragma once
#include "../AbstractCommand.h"
#include "../../Document/IDocument.h"

class RedoCommand : public AbstractCommand
{
public:
	RedoCommand(IDocument& doc);
	
private:
	void DoExecute() override;

	IDocument& m_document;
};