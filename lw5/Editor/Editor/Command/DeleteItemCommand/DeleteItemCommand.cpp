#include "DeleteItemCommand.h"

DeleteItemCommand::DeleteItemCommand(IDocument& doc, ISaver& saver, size_t index)
    : m_document(doc)
    , m_saver(saver)
    , m_deletePos(index)
{
}

void DeleteItemCommand::DoExecute()
{
    auto item = m_document.GetItem(m_deletePos);
    m_deletedItem = std::make_unique<DocumentItem>(item);

    auto image = item.GetImage();
    if (image)
    {
        m_imagePath = image->GetPath();
        m_shouldDeleteImage = true;
    }

    m_document.DeleteItem(m_deletePos);
}

void DeleteItemCommand::DoUnexecute()
{
    if (m_deletedItem->GetImage() != nullptr)
    {
        std::shared_ptr<IImage> img = m_deletedItem->GetImage();
        m_document.InsertImage(img->GetPath(), img->GetWidth(), img->GetHeight(), m_deletePos);
    }
    else
    {
        std::shared_ptr<IParagraph> paragraph = m_deletedItem->GetParagraph();
        m_document.InsertParagraph(paragraph->GetText(), m_deletePos);
    }

    m_deletedItem = nullptr;
    m_shouldDeleteImage = false;
}

void DeleteItemCommand::Destroy()
{
    if (m_shouldDeleteImage && !m_imagePath.empty())
    {
        m_saver.DeleteTempImage(m_imagePath);
    }
}