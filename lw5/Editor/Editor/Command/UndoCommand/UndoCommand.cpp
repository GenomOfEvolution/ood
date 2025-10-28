#include "UndoCommand.h"

UndoCommand::UndoCommand(IDocument& doc)
	: m_document(doc)
{
}

void UndoCommand::DoExecute()
{
	if (m_document.CanUndo())
	{
		m_document.Undo();
	}
	else
	{
		throw std::runtime_error("Can't make undo!");
	}
}