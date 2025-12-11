#include "DocumentSelection.h"
#include <algorithm>
#include <optional>

void DocumentSelection::SelectItem(const Point& point, bool additive)
{
    std::optional<size_t> hitIndex;
    for (size_t i = m_document->GetItemsCount(); i > 0; --i) 
    {
        const size_t idx = i - 1;
        auto item = m_document->GetItemAtIndex(idx);
        if (item->ContainsPoint(point)) 
        {
            hitIndex = idx;
            break;
        }
    }

    if (!additive) 
    {
        ClearSelection();
        if (hitIndex) 
        {
            m_selectedItems.push_back(*hitIndex);
        }
    }
    else if (hitIndex) 
    {
        auto& idx = *hitIndex;
        auto it = std::find(m_selectedItems.begin(), m_selectedItems.end(), idx);

        if (it != m_selectedItems.end()) 
        {
            m_selectedItems.erase(it);
        }
        else 
        {
            m_selectedItems.push_back(idx);
        }
    }
}

void DocumentSelection::AddIndex(size_t index)
{
    m_selectedItems.push_back(index);
}

void DocumentSelection::ClearSelection()
{
    m_selectedItems.clear();
}

std::vector<size_t> DocumentSelection::GetSelectedIndexes() const
{
    return m_selectedItems;
}

void DocumentSelection::SetDocument(std::shared_ptr<IDocument> doc)
{
    m_document = doc;
}
