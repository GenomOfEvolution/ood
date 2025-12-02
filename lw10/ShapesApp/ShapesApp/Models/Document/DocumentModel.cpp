#include "DocumentModel.h"
#include "../DocumentSerializer/XmlSerializer.h"
#include <iostream>

DocumentModel::DocumentModel(
	std::shared_ptr<IHistory>&& history,
	std::shared_ptr<ISerializer>&& saver)
	: m_history(std::move(history))
	, m_saver(std::move(saver))
{
}

void DocumentModel::Save()
{
    if (m_savePath.empty())
    {
        throw std::runtime_error("No save path set. Use SaveAs first.");
    }

    m_saver->Serialize(m_savePath);
}

void DocumentModel::SaveAs(const std::string& path)
{
    if (path.empty())
    {
        throw std::invalid_argument("Save path cannot be empty");
    }

    m_saver->Serialize(path);
    m_savePath = path;
}

void DocumentModel::Load(const std::string& path)
{
    if (path.empty())
    {
        throw std::invalid_argument("Load path cannot be empty");
    }

    m_saver->Deserialize(path);
    m_savePath = path;
}

void DocumentModel::AddItem(std::unique_ptr<DocumentItem>&& item)
{
	m_items.push_back(std::move(item));
}

std::shared_ptr<DocumentItem> DocumentModel::GetItemAtIndex(size_t index)
{
	return m_items.at(index);
}

std::shared_ptr<const DocumentItem> DocumentModel::GetItemAtIndex(size_t index) const
{
	return m_items.at(index);
}

void DocumentModel::RemoveItemAtIndex(size_t index)
{
	if (index < m_items.size())
	{
		m_items.erase(m_items.begin() + index);
	}
}

size_t DocumentModel::GetItemsCount() const
{
	return m_items.size();
}

bool DocumentModel::CanUndo() const
{
	return m_history->CanUndo();
}

void DocumentModel::Undo()
{
	if (CanUndo())
		m_history->Undo();
}

bool DocumentModel::CanRedo() const
{
	return m_history->CanRedo();
}

void DocumentModel::Redo()
{
	if (CanRedo())
		m_history->Redo();
}
