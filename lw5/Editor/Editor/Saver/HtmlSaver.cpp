#include "HtmlSaver.h"
#include <iostream>

HtmlSaver::HtmlSaver()
{
	SetTempPath();
}

HtmlSaver::~HtmlSaver()
{

}

void HtmlSaver::Save(const IDocument& document, const std::filesystem::path& path)
{
	std::filesystem::create_directory(path);
	std::filesystem::path imagesPath = path / "images";
	std::filesystem::create_directory(imagesPath);


}

void HtmlSaver::SaveTempImage(const IImage& image)
{

}

void HtmlSaver::DeleteTempImage(const IImage& image)
{
}

void HtmlSaver::SetTempPath()
{
    std::filesystem::path tempDir = std::filesystem::temp_directory_path();
    m_tempPath = tempDir / "document_editor_temp";

    std::filesystem::create_directories(m_tempPath);
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
