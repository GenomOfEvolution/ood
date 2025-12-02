#include "DocumentController.h"
#include "../../Models/DocumentItem/DocumentItem.h"
#include "../../Models/Command/AddShapeCommand/AddShapeCommand.h"

DocumentController::DocumentController(
	std::shared_ptr<IDocument>&& document,
	std::shared_ptr<ICommandExecutor>&& history,
	QObject* parent)
	: m_document(std::move(document))
	, m_history(std::move(history))
{
}

bool DocumentController::WasDocumentSaved() const
{
	return m_wasDocumentSaved;
}

void DocumentController::Save()
{
	if (WasDocumentSaved())
		m_document->Save();
}

void DocumentController::SaveAs(const std::string& path)
{
	m_wasDocumentSaved = true;
	m_document->SaveAs(path);
}

void DocumentController::Load(const std::string& path)
{
	m_wasDocumentSaved = true;
	m_document->Load(path);
}

bool DocumentController::CanUndo() const
{
	return m_document->CanUndo();
}

void DocumentController::Undo()
{
	m_document->Undo();
}

bool DocumentController::CanRedo() const
{
	return m_document->CanRedo();
}

void DocumentController::Redo()
{
	m_document->Redo();
}

void DocumentController::AddShape(const std::string& description)
{
	auto cmd = std::make_unique<AddShapeCommand>(*m_document, description);
	m_history->AddAndExecuteCommand(std::move(cmd));
	
	emit itemAdded(description);
}

void DocumentController::AddImageItem(const std::string& imagePath, int width, int height)
{
	//m_history->AddAndExecuteCommand();
	m_document->AddItem(std::move(m_itemFactory.CreateItem("image " + imagePath)));

	emit itemAdded("image " + imagePath);
}

void DocumentController::RemoveItemAtIndex(size_t index)
{
	//m_history->AddAndExecuteCommand();
	emit itemRemoved((int)index);
}
