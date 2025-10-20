#include "HtmlDocument.h"
#include "../../Command/InsertParagraphCommand/InsertParagraphCommand.h"
#include "../../Command/InsertImageCommand/InsertImageCommand.h"
#include "../../DocumentItem/Paragraph/Paragraph.h"
#include "../../DocumentItem/Image/CImage.h"

HtmlDocument::HtmlDocument(std::unique_ptr<IHistory> history, std::unique_ptr<ISaver> saver)
	: m_history(std::move(history))
	, m_saver(std::move(saver))
{
}

std::shared_ptr<IParagraph> HtmlDocument::InsertParagraph(const std::string& text, std::optional<size_t> position)
{
	auto paragraph = std::make_shared<Paragraph>(text);
	m_history->AddAndExecuteCommand(std::make_unique<InsertParagraphCommand>(m_items, paragraph, position));

	return paragraph;
}

std::shared_ptr<IImage> HtmlDocument::InsertImage(
	const std::filesystem::path& path,
	int width, int height,
	std::optional<size_t> position)
{
	static size_t imageCounter = 0;
	imageCounter++;

	std::string extension = path.extension().string();
	std::string filename = "image_" + std::to_string(imageCounter) + extension;

	std::filesystem::path relativePath = std::filesystem::path("images") / filename;
	auto image = std::make_shared<CImage>(relativePath, width, height);

	m_history->AddAndExecuteCommand(std::make_unique<InsertImageCommand>(m_items, image, position, path));

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
