#include "HtmlSaver.h"
#include <iostream>
#include <iterator>

HtmlSaver::HtmlSaver()
{
	SetTempPath();
}

HtmlSaver::~HtmlSaver()
{
    ClearTempFolder();
}

void HtmlSaver::Save(const IDocument& document, const std::filesystem::path& path)
{
	std::filesystem::create_directory(path);
	std::filesystem::path imagesPath = path / "images";
	std::filesystem::create_directory(imagesPath);

    std::ofstream htmlFile(path / "document.html");

    htmlFile << "<!DOCTYPE html>" << std::endl;
    htmlFile << "<html lang=\"ru\">" << std::endl;
    PrintHtmlHead(document, htmlFile);
    PrintHtmlBody(document, htmlFile);
    htmlFile << "</html>" << std::endl;
}

void HtmlSaver::SaveTempImage(const IImage& image, const std::filesystem::path& srcPath)
{
    std::filesystem::path targetDir = m_tempPath / image.GetPath().parent_path();
    std::filesystem::create_directories(targetDir);

    // Формируем полный путь для копирования
    std::filesystem::path targetPath = m_tempPath / image.GetPath();

    try 
    {
        std::filesystem::copy_file(srcPath, targetPath, std::filesystem::copy_options::overwrite_existing);
    }
    catch (const std::filesystem::filesystem_error& ex) 
    {
        throw std::runtime_error("Failed to copy image to temp directory: " + std::string(ex.what()));
    }
}

void HtmlSaver::DeleteTempImage(const IImage& image)
{
}

void HtmlSaver::SetTempPath()
{
    std::filesystem::path tempDir = std::filesystem::temp_directory_path();
    m_tempPath = tempDir / "document_editor_temp";

    std::filesystem::create_directories(m_tempPath);
    ClearTempFolder();
}

void HtmlSaver::ClearTempFolder() const
{
    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(m_tempPath))
        {
            std::filesystem::remove_all(entry.path());
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cout << e.what();
    }
}

std::string HtmlSaver::HtmlEncode(const std::string& text) const
{
    if (text.empty())
    {
        return text;
    }

    bool needsEncoding = false;
    for (char c : text) 
    {
        std::string character(1, c);
        if (HTML_ENTITY_TABLE.find(character) != HTML_ENTITY_TABLE.end()) 
        {
            needsEncoding = true;
            break;
        }
    }

    if (!needsEncoding)
    {
        return text;
    }

    std::string encoded;

    size_t currentPos = 0;
    size_t textLength = text.length();

    while (currentPos < textLength)
    {
        bool foundEntity = false;

        std::string currentChar(1, text[currentPos]);
        auto it = HTML_ENTITY_TABLE.find(currentChar);

        if (it != HTML_ENTITY_TABLE.end())
        {
            encoded += it->second;
            foundEntity = true;
        }

        if (!foundEntity)
        {
            encoded += text[currentPos];
        }

        currentPos++;
    }

    return encoded;
}

void HtmlSaver::PrintParagraph(const IParagraph& paragraph, std::ofstream& htmlFile) const
{
    htmlFile << "\t<p>" << std::endl;
    htmlFile << "\t\t" << HtmlEncode(paragraph.GetText()) << std::endl;
    htmlFile << "\t</p>" << std::endl;
}

void HtmlSaver::PrintImage(const IImage& image, std::ofstream& htmlFile) const
{
    htmlFile << "\t<img"
        << " src=\"" << image.GetPath().string() << "\""
        << " width=\"" << image.GetWidth() << "\""
        << " height=\"" << image.GetHeight() << "\""
        << " />" << std::endl;
}

void HtmlSaver::PrintHtmlHead(const IDocument& document, std::ofstream& htmlFile) const
{
    htmlFile << "<head>" << std::endl;
    htmlFile << "\t<meta charset=\"UTF - 8\">" << std::endl;

    htmlFile << "\t<title>";
    htmlFile << document.GetTitle();
    htmlFile << "</title>" << std::endl;

    htmlFile << "</head>" << std::endl;
}

void HtmlSaver::PrintHtmlBody(const IDocument& document, std::ofstream& htmlFile) const
{
    htmlFile << "<body>" << std::endl;

    for (size_t i = 0; i < document.GetItemsCount(); i++)
    {
        const DocumentItem item = document.GetItem(i);
        std::shared_ptr<const IImage> img = item.GetImage();
        std::shared_ptr<const IParagraph> paragraph = item.GetParagraph();

        if (img != nullptr)
        {
            PrintImage(*img, htmlFile);
        }

        if (paragraph != nullptr)
        {
            PrintParagraph(*paragraph, htmlFile);
        }
    }

    htmlFile << "</body>" << std::endl;
}
