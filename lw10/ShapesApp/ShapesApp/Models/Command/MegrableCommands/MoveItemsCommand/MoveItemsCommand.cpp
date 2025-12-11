#include "MoveItemsCommand.h"

MoveItemsCommand::MoveItemsCommand(
	IDocument& doc,
	ISelection& selection,
	Point delta,
	ItemsMovedCallback onItemsMoved
)
	: m_document(doc)
	, m_selection(selection)
	, m_delta(delta)
	, m_onItemsMoved(std::move(onItemsMoved))
{
	m_selectedIndexes = m_selection.GetSelectedIndexes();

	m_originalRects.reserve(m_selectedIndexes.size());
	for (auto index : m_selectedIndexes) 
	{
		auto item = m_document.GetItemAtIndex(index);
		m_originalRects.push_back(item->GetBoundingBox());
	}
}

bool MoveItemsCommand::CanMergeWith(const ICommand& other) const
{
	if (const auto* otherCmd = dynamic_cast<const MoveItemsCommand*>(&other)) 
	{
		return (otherCmd->m_selectedIndexes == m_selectedIndexes);
	}
	return false;
}

void MoveItemsCommand::MergeWith(std::unique_ptr<ICommand> other)
{
	if (auto* otherCmd = dynamic_cast<MoveItemsCommand*>(other.get())) 
	{
		m_delta.x += otherCmd->m_actualDelta.x;
		m_delta.y += otherCmd->m_actualDelta.y;
	}
}

void MoveItemsCommand::DoExecute()
{
	m_actualDelta = m_document.MoveItemsBy(m_selectedIndexes, m_delta);

	if (m_onItemsMoved && (m_actualDelta.x != 0.0 || m_actualDelta.y != 0.0)) 
	{
		m_onItemsMoved(m_selectedIndexes, m_actualDelta.x, m_actualDelta.y);
	}
}

void MoveItemsCommand::DoUnexecute()
{
	for (size_t i = 0; i < m_selectedIndexes.size(); ++i) 
	{
		auto item = m_document.GetItemAtIndex(m_selectedIndexes[i]);
		item->Resize(m_originalRects[i]);
	}

	if (m_onItemsMoved && (m_actualDelta.x != 0.0 || m_actualDelta.y != 0.0)) 
	{
		m_selection.ClearSelection();

		for (auto i : m_selectedIndexes)
		{
			m_selection.AddIndex(i);
		}

		m_onItemsMoved(m_selectedIndexes, -m_delta.x, -m_delta.y);
	}
}
