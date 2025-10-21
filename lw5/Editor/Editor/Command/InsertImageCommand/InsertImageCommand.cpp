#include "InsertImageCommand.h"

InsertImageCommand::InsertImageCommand(
	std::vector<DocumentItem>& items,
	std::shared_ptr<IImage> image,
	std::optional<size_t> index,
	std::filesystem::path path,
    ISaver& saver)
	: m_items(items)
	, m_image(std::move(image))
	, m_insertPos(index)
    , m_imgSrcPath(path)
    , m_saver(saver)
{
}

InsertImageCommand::~InsertImageCommand()
{
    Destroy();
}

void InsertImageCommand::DoExecute()
{
    // Вставить картинку в temp папку

    const DocumentItem item(m_image);

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

    m_shouldDelete = false;
}

void InsertImageCommand::DoUnexecute()
{
    // удалить картинку из temp папки

    m_shouldDelete = true;

    if (m_actualPosition >= m_items.size())
    {
        throw std::runtime_error("Cannot unexecute: position is out of range");
    }

    m_items.erase(m_items.begin() + m_actualPosition);
}

void InsertImageCommand::Destroy()
{
    if (m_shouldDelete)
    {
        // Удлить картинку из temp папки
    }

    m_shouldDelete = false;
}
