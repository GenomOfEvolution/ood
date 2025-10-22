#include "CImage.h"

CImage::CImage(std::filesystem::path path, int width, int height)
    : m_path(path)
    , m_width(width)
    , m_height(height)
{
}

std::filesystem::path CImage::GetPath() const
{
    return m_path;
}

void CImage::SetPath(const std::filesystem::path& path)
{
    m_path = path;
}

int CImage::GetWidth() const
{
    return m_width;
}

int CImage::GetHeight() const
{
    return m_height;
}

void CImage::Resize(int width, int height)
{
    if (!IsCorrectSize(width) || !IsCorrectSize(height))
    {
        throw std::out_of_range("Can't resize!\n Min img size: " + std::to_string(MIN_IMAGE_SIZE) 
            + "px, Max img size: " + std::to_string(MAX_IMAGE_SIZE) + "px");
    }

    m_width = width;
    m_height = height;
}

bool CImage::IsCorrectSize(int size) const
{
    return size >= MIN_IMAGE_SIZE && size <= MAX_IMAGE_SIZE;
}
