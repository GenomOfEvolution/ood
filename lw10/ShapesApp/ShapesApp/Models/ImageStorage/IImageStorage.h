#pragma once
#include <filesystem>

class IImageStorage
{
public:
	virtual ~IImageStorage() = default;

	virtual std::string SaveImage(const std::string& srcPath) = 0;
	virtual void DeleteTempImage(const std::string& srcPath) = 0;
	virtual void CopyAllImagesFromStorage(const std::string& copyTo) = 0;

	virtual void SetTempDirPath(const std::filesystem::path& path) = 0;
	virtual std::filesystem::path GetTempDirPath() const = 0;
	virtual void ClearStorage() = 0;
};