#include "InsertImageCommand.h"

InsertImageCommand::InsertImageCommand(
    IDocument& doc,
    ISaver& saver,
    std::optional<size_t> index,
    int width,
    int height,
    std::filesystem::path sourcePath)
	: m_document(doc)
	, m_saver(saver)
	, m_insertPos(index)
    , m_width(width)
    , m_height(height)
    , m_imgSrcPath(sourcePath)
{
}

InsertImageCommand::~InsertImageCommand()
{
    Destroy();
}

void InsertImageCommand::DoExecute()
{
    m_tempPath = m_saver.SaveTempImage(m_imgSrcPath);
    m_image = m_document.InsertImage(m_tempPath, m_width, m_height, m_insertPos);

    if (m_insertPos.has_value()) 
    {
        m_actualPosition = m_insertPos.value();
    }
    else 
    {
        m_actualPosition = m_document.GetItemsCount() - 1;
    }

    m_shouldDelete = false;
}

void InsertImageCommand::DoUnexecute()
{
    if (m_actualPosition >= m_document.GetItemsCount())
    {
        throw std::runtime_error("Cannot unexecute: position is out of range");
    }

    m_shouldDelete = true;
    m_document.DeleteItem(m_actualPosition);
}

void InsertImageCommand::Destroy()
{
    if (m_shouldDelete && !m_tempPath.empty()) 
    {
        m_saver.DeleteTempImage(m_tempPath);
        m_tempPath.clear();
    }
}
