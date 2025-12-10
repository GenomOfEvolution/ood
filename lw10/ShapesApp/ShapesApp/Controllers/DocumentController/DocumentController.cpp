#include "DocumentController.h"
#include "../../Models/DocumentItem/DocumentItem.h"
#include "../../Models/Command/AddShapeCommand/AddShapeCommand.h"
#include "../../Models/Command/AddImageCommand/AddImageCommand.h"
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
	auto cmd = std::make_unique<AddShapeCommand>(*m_document, description);
	m_history->AddAndExecuteCommand(std::move(cmd));
	
	auto prview = m_document->GetItemAtIndex(m_document->GetItemsCount() - 1)->GetPreview();

	prview.m_index = m_document->GetItemsCount() - 1;

	emit itemAdded(prview);
}

void DocumentController::AddImageItem(const std::filesystem::path& imagePath, double width, double height)
{
	auto cmd = std::make_unique<AddImageCommand>(*m_document, *m_storage, imagePath, width, height);
	m_history->AddAndExecuteCommand(std::move(cmd));

	DocItemPreview preview
	{
		.m_type = DocItemPreview::ItemType::Image,
		.m_boundingBox = Rect{ 0, 0, width, height },
		.m_imgPath = imagePath.string(),
		.m_index = m_document->GetItemsCount() - 1
	};

	emit itemAdded(preview);
}

std::vector<size_t> DocumentController::GetSelectedIndexes() const
{
	return m_selection->GetSelectedIndexes();
}

void DocumentController::RemoveSelectedItems()
{
	m_dragging = false;
	auto indexes = m_selection->GetSelectedIndexes();
	std::sort(indexes.rbegin(), indexes.rend(), std::greater<size_t>());

	for (size_t index : indexes) 
	{
		m_document->RemoveItemAtIndex(index);
	}
	emit itemsRemoved(indexes);

	m_selection->ClearSelection();
	emit selectionChanged();
}

void DocumentController::Resize(HandleType type, double dx, double dy)
{
	std::vector<QRectF> newBoundingBoxes;
	auto indexes = m_selection->GetSelectedIndexes();
	auto newBBoxes = m_document->ResizeItemsBy(indexes, { dx, dy }, type);

	for (const auto& bbox : newBBoxes)
	{
		newBoundingBoxes.push_back(QRectF{ bbox.x, bbox.y, bbox.width, bbox.height });
	}

	emit itemsResized(newBoundingBoxes);
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

        Point actualDelta = m_document->MoveItemsBy(m_selection->GetSelectedIndexes(), delta);

        m_dragStartPoint = m_dragStartPoint + actualDelta;

        emit itemsMoved(m_selection->GetSelectedIndexes(), actualDelta.x, actualDelta.y);
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