#pragma once
#include "../AbstractUndoableCommand.h"
#include "../../Document/IDocument.h"
#include "../../Selection/ISelection.h"

class DeleteItemsCommand : public AbstractUndoableCommand
{
public:
	DeleteItemsCommand();

private:
	void DoExecute() override;
	void DoUnexecute() override;
	void Destroy();
};