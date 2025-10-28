#include "ListCommand.h"
#include <iostream>

ListCommand::ListCommand(IDocument& doc)
	: m_document(doc)
{
}

void ListCommand::DoExecute()
{
	std::cout << "Title: " << m_document.GetTitle() << std::endl;

	for (size_t i = 0; i < m_document.GetItemsCount(); i++)
	{
		std::cout << i + 1 << ". ";
		const auto item = m_document.GetItem(i);
		if (item.GetImage() != nullptr)
		{
			PrintImage(*item.GetImage());
		}
		else
		{
			PrintParagraph(*item.GetParagraph());
		}

		std::cout << std::endl;
	}
}

void ListCommand::PrintParagraph(const IParagraph& paragraph) const
{
	std::cout << "Paragraph: " << paragraph.GetText();
}

void ListCommand::PrintImage(const IImage& image) const
{
	std::cout << "Image: " << image.GetWidth() << " " << image.GetHeight() << " " << image.GetPath();
}
