#include "AddShapeCommand.h"
#include "../../Factory/DocItemFactory/DocItemFactory.h"

AddShapeCommand::AddShapeCommand(IDocument& doc, const std::string& description)
	: m_document(doc)
	, m_shapeDescription(description)
{
}

void AddShapeCommand::DoExecute()
{
	m_insertPos = m_document.GetItemsCount();
	m_document.AddItem(DocItemFactory::CreateItem(m_shapeDescription));
}

void AddShapeCommand::DoUnexecute()
{
	m_document.RemoveItemAtIndex(m_insertPos);
}
