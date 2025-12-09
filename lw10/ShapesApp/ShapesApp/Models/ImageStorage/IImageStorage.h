#pragma once
#include <filesystem>

class IImageStorage
{
public:
	virtual ~IImageStorage() = default;

	virtual void CopyAllImagesToStorage(const std::filesystem::path& srcPath) = 0;
	virtual std::string SaveImage(const std::filesystem::path& srcPath) = 0;
	virtual void DeleteTempImage(const std::filesystem::path& srcPath) = 0;
	virtual void CopyAllImagesFromStorage(const std::filesystem::path& copyTo) = 0;

	virtual void SetTempDirPath(const std::filesystem::path& path) = 0;
	virtual std::filesystem::path GetTempDirPath() const = 0;
	virtual void ClearStorage() = 0;
};