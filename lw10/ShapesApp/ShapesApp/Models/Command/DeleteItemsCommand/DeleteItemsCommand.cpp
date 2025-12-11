#include "DeleteItemsCommand.h"

DeleteItemsCommand::DeleteItemsCommand(
    IDocument& doc,
    ISelection& selection,
    IImageStorage& storage,
    DocumentChangedCallback onDocumentChanged
)
    : m_document(doc)
    , m_selection(selection)
    , m_storage(storage)
    , m_onDocumentChanged(std::move(onDocumentChanged))
{
    m_deletedIndexes = m_selection.GetSelectedIndexes();
    std::sort(m_deletedIndexes.begin(), m_deletedIndexes.end(), std::greater<size_t>());
}

void DeleteItemsCommand::Destroy()
{
    for (auto& data : m_deletedItems)
    {
        if (auto image = data.item->GetImage())
        {
            m_storage.DeleteTempImage(image->GetPath());
        }
    }
}

DeleteItemsCommand::~DeleteItemsCommand()
{
    Destroy();
}

void DeleteItemsCommand::DoExecute()
{
    m_deletedItems.reserve(m_deletedIndexes.size());
    for (auto index : m_deletedIndexes) 
    {
        auto item = m_document.GetItemAtIndex(index);
        DeletedItemData data;
        data.originalIndex = index;
        data.item = item;
        m_deletedItems.push_back(std::move(data));
    }

    for (auto index : m_deletedIndexes) 
    {
        m_document.RemoveItemAtIndex(index);
    }
    m_selection.ClearSelection();

    if (m_onDocumentChanged) 
    {
        m_onDocumentChanged();
    }

    m_shouldDeleteImages = true;
}

void DeleteItemsCommand::DoUnexecute()
{
    std::sort(m_deletedItems.begin(), m_deletedItems.end(),
    [](const auto& a, const auto& b) 
    {
        return a.originalIndex < b.originalIndex;
    });

    for (const auto& data : m_deletedItems)
    {
        if (data.item->GetImage())
        {
            auto image = data.item->GetImage();
            m_document.InsertItemAtIndex(std::make_unique<DocumentItem>(image), data.originalIndex);
        }
        else
        {
            auto shape = data.item->GetShape();
            m_document.InsertItemAtIndex(std::make_unique<DocumentItem>(shape), data.originalIndex);
        }
    }

    for (const auto& data : m_deletedItems) 
    {
        m_selection.AddIndex(data.originalIndex);
    }

    if (m_onDocumentChanged) 
    {
        m_onDocumentChanged();
    }

    m_shouldDeleteImages = false;
}
