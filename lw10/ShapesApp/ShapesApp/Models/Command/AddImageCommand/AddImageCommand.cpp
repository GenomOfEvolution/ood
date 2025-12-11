#include "AddImageCommand.h"
#include "../../DocumentItem/Image/CImage.h"

AddImageCommand::AddImageCommand(
	IDocument& doc,
	ISelection& selection,
	IImageStorage& storage,
	const std::filesystem::path& srcPath, double width, double height,
	ImageAddedCallback onImageAdded,
	ImageRemovedCallback onImageRemoved
)
	: m_document(doc)
	, m_storage(storage)
	, m_selection(selection)
	, m_srcPath(srcPath)
	, m_width(width), m_height(height)
	, m_onImageAdded(std::move(onImageAdded))
	, m_onImageRemoved(std::move(onImageRemoved))
{
}

AddImageCommand::~AddImageCommand()
{
	Destroy();
}

void AddImageCommand::DoExecute()
{
	m_tempPath = m_storage.SaveImage(m_srcPath);
	m_insertPos = m_document.GetItemsCount();

	m_image = std::make_shared<CImage>(m_tempPath, Point{ 0, 0 }, m_width, m_height);
	m_document.AddItem(std::make_unique<DocumentItem>(m_image));
	
	m_selection.ClearSelection();
	m_selection.AddIndex(m_insertPos);

	m_shouldDelete = false;

	if (m_onImageAdded) 
	{
		m_onImageAdded();
	}
}

void AddImageCommand::DoUnexecute()
{
	m_shouldDelete = true;
	m_document.RemoveItemAtIndex(m_insertPos);
	m_selection.ClearSelection();

	if (m_onImageRemoved)
	{
		m_onImageRemoved();
	}

}

void AddImageCommand::Destroy()
{
	if (m_shouldDelete && !m_tempPath.empty())
	{
		m_storage.DeleteTempImage(m_tempPath);
		m_tempPath.clear();
	}
}
