#include "ImageStorage.h"
#include <iostream>
#include <iterator>
#include <random>

ImageStorage::ImageStorage()
{
	CreateTempDirectory();
	ClearTempDirectory();
}

ImageStorage::ImageStorage(const std::string& tempDirPath)
	: m_storageDir(tempDirPath)
{
	CreateTempDirectory();
	ClearTempDirectory();
}

void ImageStorage::CopyAllImagesToStorage(const std::filesystem::path& srcPath)
{
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(srcPath.string()))
		{
			std::filesystem::copy(entry.path(), m_storageDir, std::filesystem::copy_options::overwrite_existing);
		}
	}
	catch (const std::filesystem::filesystem_error& ex)
	{
		std::cerr << "Failed to copy image to temp directory: " + std::string(ex.what()) << std::endl;
	}
}

std::string ImageStorage::SaveImage(const std::filesystem::path& srcPath)
{
	std::filesystem::path imgPath = srcPath;

	std::string extension = imgPath.extension().string();
	std::string filename = GenerateUniqueFilename() + extension;

	std::filesystem::path targetPath = m_storageDir / filename;

	try
	{
		std::filesystem::copy_file(srcPath, targetPath, std::filesystem::copy_options::overwrite_existing);

		return "images/" + filename;
	}
	catch (const std::filesystem::filesystem_error& ex)
	{
		throw std::runtime_error("Failed to copy image to temp directory: " + std::string(ex.what()));
	}
}

void ImageStorage::DeleteTempImage(const std::filesystem::path& srcPath)
{
	try
	{
		std::filesystem::path fullPath = m_storageDir / srcPath.filename();

		if (std::filesystem::exists(fullPath))
		{
			std::filesystem::remove(fullPath);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Image storage: " << e.what() << "\n";
	}
}

void ImageStorage::CopyAllImagesFromStorage(const std::filesystem::path& copyTo)
{
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(m_storageDir))
		{
			std::filesystem::copy(entry.path(), copyTo);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}
}

void ImageStorage::SetTempDirPath(const std::filesystem::path& path)
{
	m_storageDir = path;
}

std::filesystem::path ImageStorage::GetTempDirPath() const
{
	return m_storageDir;
}

void ImageStorage::ClearStorage()
{
	ClearTempDirectory();
}

ImageStorage::~ImageStorage()
{
	ClearTempDirectory();
}

void ImageStorage::CreateTempDirectory()
{
	try
	{
		m_storageDir = m_storageDir / "ShapesApp_temp";
		std::filesystem::create_directories(m_storageDir);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}
}

void ImageStorage::ClearTempDirectory()
{
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(m_storageDir))
		{
			std::filesystem::remove_all(entry.path());
		}
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << "\n";
	}
}

std::string ImageStorage::GenerateUniqueFilename() const
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dis(100000, 999999);

	return "image_" + std::to_string(dis(gen));
}