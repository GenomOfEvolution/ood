#pragma once
#include "../AbstractCommand.h"
#include "../../Document/IDocument.h"

class UndoCommand : public AbstractCommand
{
public:
	UndoCommand(IDocument& doc);

private:
	void DoExecute() override;

	IDocument& m_document;
};