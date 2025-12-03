#include "DocumentController.h"
#include "../../Models/DocumentItem/DocumentItem.h"
#include "../../Models/Command/AddShapeCommand/AddShapeCommand.h"
#include "../../Models/Command/AddImageCommand/AddImageCommand.h"

DocumentController::DocumentController(
	std::shared_ptr<IDocument>&& document,
	std::shared_ptr<ICommandExecutor>&& history,
	std::shared_ptr<IImageStorage>&& storage,
	QObject* parent)
	: m_document(std::move(document))
	, m_history(std::move(history))
	, m_storage(std::move(storage))
{
}

DocItemPreview DocumentController::ConvertToDTO(size_t index)
{
	auto item = m_document->GetItemAtIndex(index);

	if (item->GetImage())
	{
		return item->GetImage()->GetPreview();
	}
	else
	{
		return item->GetShape()->GetPreview();
	}

	return DocItemPreview();
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
	if (!m_document->GetSavePath().empty())
		m_storage->CopyAllImagesToStorage(m_document->GetSavePath() + "/images");

	m_wasDocumentSaved = true;
	m_document->SaveAs(path);
}

void DocumentController::Load(const std::string& path)
{
	m_wasDocumentSaved = true;
	m_document->Load(path);
	m_history->Clear();

	emit documentLoaded();

	for (size_t i = 0; i < m_document->GetItemsCount(); i++)
	{
		auto preview = m_document->GetItemAtIndex(i)->GetPreview();
		preview.m_imgPath = m_document->GetSavePath() + "/" + preview.m_imgPath;

		emit itemAdded(preview);
	}
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
	
	emit itemAdded(m_document->GetItemAtIndex(m_document->GetItemsCount() - 1)->GetPreview());
}

void DocumentController::AddImageItem(const std::string& imagePath, double width, double height)
{
	auto cmd = std::make_unique<AddImageCommand>(*m_document, *m_storage, imagePath, width, height);
	m_history->AddAndExecuteCommand(std::move(cmd));

	DocItemPreview preview
	{
		.m_type = DocItemPreview::ItemType::Image,
		.m_boundingBox = Rect{ 0, 0, width, height },
		.m_imgPath = imagePath,
	};

	emit itemAdded(preview);
}

void DocumentController::RemoveItemAtIndex(size_t index)
{
	//m_history->AddAndExecuteCommand();
	emit itemRemoved((int)index);
}
