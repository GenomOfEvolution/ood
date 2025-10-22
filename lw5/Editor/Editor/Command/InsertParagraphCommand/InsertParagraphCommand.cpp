#include "InsertParagraphCommand.h"
#include <iostream>

InsertParagraphCommand::InsertParagraphCommand(
    IDocument& doc,
    std::optional<size_t> index,
    std::string text)
	: m_document(doc)
	, m_text(text)
	, m_insertPos(index)
{
}

void InsertParagraphCommand::DoExecute()
{
    if (m_insertPos.has_value())
    {
        if (*m_insertPos > m_document.GetItemsCount())
        {
            throw std::out_of_range("Insert position is out of range");
        }

        m_document.InsertParagraph(m_text, m_actualPosition);
        m_actualPosition = *m_insertPos; 
    }
    else
    {
        m_document.InsertParagraph(m_text, m_actualPosition);
        m_actualPosition = m_document.GetItemsCount() - 1;
    }
}

void InsertParagraphCommand::DoUnexecute()
{
    if (m_actualPosition >= m_document.GetItemsCount())
    {
        throw std::runtime_error("Cannot unexecute: position is out of range");
    }

    m_document.DeleteItem(m_actualPosition);
}
