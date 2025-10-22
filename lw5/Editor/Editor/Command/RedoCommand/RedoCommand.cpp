#include "RedoCommand.h"

RedoCommand::RedoCommand(IDocument& doc)
	: m_document(doc)
{
}

void RedoCommand::DoExecute()
{
	if (m_document.CanRedo())
	{
		m_document.Redo();
	}
	else
	{
		throw std::runtime_error("Can't make redo!");
	}
}

void RedoCommand::DoUnexecute()
{
}
