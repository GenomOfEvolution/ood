#pragma once
#include "../AbstractUndoableCommand.h"
#include "../../Document/IDocument.h"
#include "../../Selection/ISelection.h"
#include "../../ImageStorage/IImageStorage.h"
#include <vector>

using DocumentChangedCallback = std::function<void()>;

class DeleteItemsCommand : public AbstractUndoableCommand
{
public:
    DeleteItemsCommand(
        IDocument& doc,
        ISelection& selection,
        IImageStorage& storage,
        DocumentChangedCallback onDocumentChanged = nullptr
    );

    ~DeleteItemsCommand() override;

private:
    void DoExecute() override;
    void DoUnexecute() override;
    void Destroy();

    IDocument& m_document;
    ISelection& m_selection;
    IImageStorage& m_storage;
    DocumentChangedCallback m_onDocumentChanged;

    struct DeletedItemData 
    {
        size_t originalIndex = 0;
        std::shared_ptr<DocumentItem> item;
    };

    std::vector<DeletedItemData> m_deletedItems;
    std::vector<size_t> m_deletedIndexes;
    bool m_shouldDeleteImages = false;
};