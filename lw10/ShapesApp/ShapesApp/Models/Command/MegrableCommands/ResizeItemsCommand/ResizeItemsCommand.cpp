#include "ResizeItemsCommand.h"

ResizeItemsCommand::ResizeItemsCommand(
    IDocument& doc,
    ISelection& selection,
    HandleType type,
    Point delta,
    ItemsResizedCallback onItemsResized)
    : m_document(doc)
    , m_selection(selection)
    , m_type(type)
    , m_delta(delta)
    , m_onItemsResized(std::move(onItemsResized))
{
    m_selectedIndexes = m_selection.GetSelectedIndexes();
    m_oldBboxes.reserve(m_selectedIndexes.size());
    for (auto index : m_selectedIndexes) 
    {
        auto item = m_document.GetItemAtIndex(index);
        m_oldBboxes.push_back(item->GetBoundingBox());
    }
}

bool ResizeItemsCommand::CanMergeWith(const ICommand& other) const
{
    if (const auto* otherCmd = dynamic_cast<const ResizeItemsCommand*>(&other)) 
    {
        return (
            otherCmd->m_type == m_type &&
            otherCmd->m_selectedIndexes == m_selectedIndexes
        );
    }

    return false;
}

void ResizeItemsCommand::MergeWith(std::unique_ptr<ICommand> other)
{
    if (auto* otherCmd = dynamic_cast<ResizeItemsCommand*>(other.get())) 
    {
        m_delta.x += otherCmd->m_delta.x;
        m_delta.y += otherCmd->m_delta.y;
    }
}

void ResizeItemsCommand::DoExecute()
{
    m_newBBoxes = m_document.ResizeItemsBy(m_selectedIndexes, m_delta, m_type);

    if (m_onItemsResized) 
    {
        m_onItemsResized(m_newBBoxes);
    }
}

void ResizeItemsCommand::DoUnexecute()
{
    for (size_t i = 0; i < m_selectedIndexes.size(); ++i) 
    {
        auto item = m_document.GetItemAtIndex(m_selectedIndexes[i]);
        item->Resize(m_oldBboxes[i]);
    }

    m_selection.ClearSelection();
    for (auto i : m_selectedIndexes)
    {
        m_selection.AddIndex(i);
    }

    if (m_onItemsResized) 
    {
        m_onItemsResized(m_oldBboxes);
    }
}
