#include "InsertParagraphCommand.h"
#include <iostream>

InsertParagraphCommand::InsertParagraphCommand(
	std::vector<DocumentItem>& items,
	std::shared_ptr<IParagraph> newItem,
	std::optional<size_t> index)
	: m_items(items)
	, m_newItem(std::move(newItem))
	, m_insertPos(index)
{
}

void InsertParagraphCommand::DoExecute()
{
    const DocumentItem item(m_newItem);

    if (m_insertPos.has_value())
    {
        if (*m_insertPos > m_items.size())
        {
            throw std::out_of_range("Insert position is out of range");
        }

        m_items.insert(m_items.begin() + *m_insertPos, item);
        m_actualPosition = *m_insertPos; 
    }
    else
    {
        m_items.push_back(item);
        m_actualPosition = m_items.size() - 1; 
    }
}

void InsertParagraphCommand::DoUnexecute()
{
    if (m_actualPosition >= m_items.size())
    {
        throw std::runtime_error("Cannot unexecute: position is out of range");
    }

    m_items.erase(m_items.begin() + m_actualPosition);
}
