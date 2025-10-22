#pragma once
#include "../AbstractCommand.h"
#include "../../Document/IDocument.h"
#include "../../DocumentItem/DocumentItem.h"
#include "../../Saver/ISaver.h"
#include <memory>

class DeleteItemCommand : public AbstractCommand
{
public:
    DeleteItemCommand(IDocument& doc, ISaver& saver, size_t index);

private:
    void DoExecute() override;
    void DoUnexecute() override;
    void Destroy();

    IDocument& m_document;
    ISaver& m_saver;
    size_t m_deletePos;
    std::unique_ptr<DocumentItem> m_deletedItem = nullptr;
    bool m_shouldDeleteImage = false;
    std::filesystem::path m_imagePath;
};