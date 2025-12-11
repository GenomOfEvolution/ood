#include "DocumentController.h"
#include "../../Models/DocumentItem/DocumentItem.h"
#include "../../Models/Command/AddShapeCommand/AddShapeCommand.h"
#include "../../Models/Command/AddImageCommand/AddImageCommand.h"
#include "../../Models/Command/MegrableCommands/ResizeItemsCommand/ResizeItemsCommand.h"
#include "../../Models/Command/MegrableCommands/MoveItemsCommand/MoveItemsCommand.h"
#include "../../Models/Command/DeleteItemsCommand/DeleteItemsCommand.h"

#include <qdebug.h>
#include <algorithm>

DocumentController::DocumentController(
	std::shared_ptr<IDocument>&& document,
	std::shared_ptr<ICommandExecutor>&& history,
	std::shared_ptr<IImageStorage>&& storage,
	std::shared_ptr<ISelection>&& selection,
	QObject* parent)
	: m_document(std::move(document))
	, m_history(std::move(history))
	, m_storage(std::move(storage))
	, m_selection(std::move(selection))
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
	m_selection->ClearSelection();

	emit selectionChanged();
	emit documentLoaded();

	for (size_t i = 0; i < m_document->GetItemsCount(); i++)
	{
		auto preview = m_document->GetItemAtIndex(i)->GetPreview();
		preview.m_imgPath = m_document->GetSavePath() + "/" + preview.m_imgPath;
		preview.m_index = i;

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
	auto onShapeAdded = [this]() 
	{
		auto item = m_document->GetItemAtIndex(m_document->GetItemsCount() - 1);
		auto preview = item->GetPreview();
		preview.m_index = m_document->GetItemsCount() - 1;
		emit itemAdded(preview);
	};

	auto onShapeRemoved = [this]()
	{
		emit deleteLastItem(m_document->GetItemsCount());
	};

	auto command = std::make_unique<AddShapeCommand>(
		*m_document,
		*m_selection,
		description,
		std::move(onShapeAdded),
		std::move(onShapeRemoved)
	);

	m_history->AddAndExecuteCommand(std::move(command));
}

void DocumentController::AddImageItem(const std::filesystem::path& imagePath, double width, double height)
{
	ImageAddedCallback onImageAdded = [this]()
	{
		auto item = m_document->GetItemAtIndex(m_document->GetItemsCount() - 1);
		auto preview = item->GetPreview();
		preview.m_index = m_document->GetItemsCount() - 1;
		emit itemAdded(preview);
	};

	ImageRemovedCallback onImageRemoved = [this]()
	{
		emit deleteLastItem(m_document->GetItemsCount());
	};

	auto cmd = std::make_unique<AddImageCommand>(
		*m_document,
		*m_selection,
		*m_storage,
		imagePath,
		width, height,
		std::move(onImageAdded),
		std::move(onImageRemoved)
	);

	m_history->AddAndExecuteCommand(std::move(cmd));
}

std::vector<size_t> DocumentController::GetSelectedIndexes() const
{
	return m_selection->GetSelectedIndexes();
}

std::shared_ptr<IImageStorage> DocumentController::GetImageStorage() const
{
	return m_storage;
}

void DocumentController::RemoveSelectedItems()
{
	if (m_selection->GetSelectedIndexes().empty())
		return;

	auto callback = [this]() 
	{
		emit documentChanged();
	};

	auto command = std::make_unique<DeleteItemsCommand>(
		*m_document,
		*m_selection,
		*m_storage,
		std::move(callback)
	);

	m_history->AddAndExecuteCommand(std::move(command));
	m_dragging = false;
}

void DocumentController::Resize(HandleType type, double dx, double dy)
{
	if (m_selection->GetSelectedIndexes().empty())
		return;

	ItemsResizedCallback callback = [this](const std::vector<Rect>& newBoundingBoxes)
	{
		std::vector<QRectF> qtBoxes;
		qtBoxes.reserve(newBoundingBoxes.size());
		for (const auto& bbox : newBoundingBoxes) 
		{
			qtBoxes.emplace_back(bbox.x, bbox.y, bbox.width, bbox.height);
		}
		emit itemsResized(qtBoxes);
	};

	auto command = std::make_unique<ResizeItemsCommand>(
		*m_document,
		*m_selection,
		type,
		Point{ dx, dy },
		std::move(callback)
	);

	m_history->AddAndExecuteCommand(std::move(command));
}

std::shared_ptr<const DocumentItem> DocumentController::GetItemAtIndex(size_t index) const
{
	return m_document->GetItemAtIndex(index);
}

size_t DocumentController::GetItemsCount() const
{
	return m_document->GetItemsCount();
}

bool DocumentController::IsPointOverSelectedItem(const Point& point) const
{
	for (auto index : m_selection->GetSelectedIndexes()) 
	{
		auto item = m_document->GetItemAtIndex(index);
		if (item->ContainsPoint(point)) 
		{
			return true;
		}
	}
	return false;
}

void DocumentController::handleMousePress(const QPointF& scenePos, Qt::KeyboardModifiers modifiers)
{
	bool ctrlPressed = modifiers.testFlag(Qt::ControlModifier);
	Point clickPoint(scenePos.x(), scenePos.y());

	bool clickedOnSelectedItem = IsPointOverSelectedItem(clickPoint);
	auto indexes = m_selection->GetSelectedIndexes();

	if (clickedOnSelectedItem && !ctrlPressed) 
	{
		m_dragging = true;
		m_dragStartPoint = clickPoint;
	}
	else 
	{
		m_selection->SelectItem(clickPoint, ctrlPressed);
		m_dragging = false;

		emit selectionChanged();

		if (!m_selection->GetSelectedIndexes().empty() && IsPointOverSelectedItem(clickPoint)) 
		{
			m_dragging = true;
			m_dragStartPoint = clickPoint;
		}
	}
}

void DocumentController::handleMouseMove(const QPointF& scenePos, Qt::KeyboardModifiers modifiers)
{
	if (m_dragging) 
	{
		Point currentPoint(scenePos.x(), scenePos.y());
		Point delta = currentPoint - m_dragStartPoint;

		if (delta.x == 0.0 && delta.y == 0.0) 
			return;

		ItemsMovedCallback callback = [this](const std::vector<size_t>& indexes, double dx, double dy)
		{
			m_dragStartPoint += {dx, dy};
			emit itemsMoved(indexes, dx, dy);
		};

		auto command = std::make_unique<MoveItemsCommand>(
			*m_document,
			*m_selection,
			delta,
			std::move(callback)
		);

		m_history->AddAndExecuteCommand(std::move(command));
	}
}

void DocumentController::handleMouseRelease(const QPointF& scenePos, Qt::KeyboardModifiers modifiers)
{
	if (m_dragging) 
	{
		m_dragging = false;
		emit selectionChanged();
	}
}