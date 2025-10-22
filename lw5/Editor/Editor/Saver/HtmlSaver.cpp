#include "HtmlSaver.h"
#include <iostream>
#include <iterator>
#include <random>

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

    if (std::filesystem::exists(imagesPath))
    {
        std::filesystem::remove_all(imagesPath);
    }

	std::filesystem::create_directory(imagesPath);

    CopyTempImagesToFinal(imagesPath);

    std::ofstream htmlFile(path / "document.html");

    htmlFile << "<!DOCTYPE html>" << std::endl;
    htmlFile << "<html lang=\"ru\">" << std::endl;
    PrintHtmlHead(document, htmlFile);
    PrintHtmlBody(document, htmlFile);
    htmlFile << "</html>" << std::endl;
}

std::string HtmlSaver::GenerateUniqueFilename()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(100000, 999999);

    return "image_" + std::to_string(dis(gen));
}

std::filesystem::path HtmlSaver::SaveTempImage(const std::filesystem::path& srcPath)
{
    std::filesystem::path tempImagesDir = m_tempPath / "images";
    std::filesystem::create_directories(tempImagesDir);

    std::string extension = srcPath.extension().string();
    std::string filename = GenerateUniqueFilename() + extension;
    std::filesystem::path targetPath = tempImagesDir / filename;

    try
    {
        std::filesystem::copy_file(srcPath, targetPath, std::filesystem::copy_options::overwrite_existing);

        return std::filesystem::path("images") / filename;
    }
    catch (const std::filesystem::filesystem_error& ex)
    {
        throw std::runtime_error("Failed to copy image to temp directory: " + std::string(ex.what()));
    }
}

void HtmlSaver::DeleteTempImage(const std::filesystem::path& srcPath)
{
    try
    {
        std::filesystem::path fullPath = m_tempPath / srcPath;

        if (std::filesystem::exists(fullPath))
        {
            std::filesystem::remove(fullPath);
        }
    }
    catch (const std::filesystem::filesystem_error& ex)
    {
        std::cout << "Failed to delete temp image: " << ex.what() << std::endl;
    }
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

void HtmlSaver::CopyTempImagesToFinal(const std::filesystem::path& finalImagesDir)
{
    try
    {
        std::filesystem::path tempImagesDir = m_tempPath / "images";

        if (std::filesystem::exists(tempImagesDir))
        {
            for (const auto& entry : std::filesystem::directory_iterator(tempImagesDir))
            {
                if (entry.is_regular_file())
                {
                    std::filesystem::path targetPath = finalImagesDir / entry.path().filename();
                    std::filesystem::copy_file(entry.path(), targetPath,
                        std::filesystem::copy_options::overwrite_existing);
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& ex)
    {
        throw std::runtime_error("Failed to copy images to final directory: " + std::string(ex.what()));
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
