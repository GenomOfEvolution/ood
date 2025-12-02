#pragma once
#include "../../Document/IDocument.h"
#include "../AbstractUndoableCommand.h"

class AddShapeCommand : public AbstractUndoableCommand
{
public:
	AddShapeCommand(IDocument& doc, const std::string& description);

private:
	void DoExecute() override;
	void DoUnexecute() override;

	IDocument& m_document;
	std::string m_shapeDescription;
	size_t m_insertPos = 0;
};