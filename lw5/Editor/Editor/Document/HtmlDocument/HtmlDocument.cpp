#include "HtmlDocument.h"
#include "../../DocumentItem/Image/CImage.h"
#include "../../DocumentItem/Paragraph/Paragraph.h"

HtmlDocument::HtmlDocument(std::shared_ptr<IHistory> history, std::shared_ptr<ISaver> saver)
	: m_history(std::move(history))
	, m_saver(std::move(saver))
{
}

std::shared_ptr<IParagraph> HtmlDocument::InsertParagraph(const std::string& text, std::optional<size_t> position)
{
	auto paragraph = std::make_shared<Paragraph>();
	paragraph->SetText(text);

	InsertDocumentItem(DocumentItem(paragraph), position);
	
	return paragraph;
}

std::shared_ptr<IImage> HtmlDocument::InsertImage(
	const std::filesystem::path& path,
	int width, int height,
	std::optional<size_t> position)
{
	auto image = std::make_shared<CImage>();
	image->Resize(width, height);
	image->SetPath(path);

	InsertDocumentItem(DocumentItem(image), position);

	return image;
}

size_t HtmlDocument::GetItemsCount() const
{
	return m_items.size();
}

DocumentItem HtmlDocument::GetItem(size_t index) const
{
	return m_items.at(index);
}

DocumentItem HtmlDocument::GetItem(size_t index)
{
	return m_items.at(index);
}

void HtmlDocument::DeleteItem(size_t index)
{
	m_items.erase(m_items.begin() + index);
}

std::string HtmlDocument::GetTitle() const
{
	return m_title;
}

void HtmlDocument::SetTitle(const std::string& title)
{
	m_title = title;
}

bool HtmlDocument::CanUndo() const
{
	return m_history->CanUndo();
}

void HtmlDocument::Undo()
{
	m_history->Undo();
}

bool HtmlDocument::CanRedo() const
{
	return m_history->CanRedo();
}

void HtmlDocument::Redo()
{
	m_history->Redo();
}

void HtmlDocument::Save(const std::filesystem::path& path) const
{
	m_saver->Save(*this, path);
}

void HtmlDocument::InsertDocumentItem(DocumentItem item, std::optional<size_t> position)
{
	if (position.has_value())
	{
		m_items.insert(m_items.begin() + *position, item);
	}
	else
	{
		m_items.push_back(item);
	}
}
