#pragma once
#include "IImageStorage.h"

class ImageStorage : public IImageStorage
{
public:
	ImageStorage();
	ImageStorage(const std::string& tempDirPath);

	void CopyAllImagesToStorage(const std::filesystem::path& srcPath) override;
	std::string SaveImage(const std::filesystem::path& srcPath) override;
	void DeleteTempImage(const std::filesystem::path& srcPath) override;
	void CopyAllImagesFromStorage(const std::filesystem::path& copyTo) override;

	void SetTempDirPath(const std::filesystem::path& path) override;
	std::filesystem::path GetTempDirPath() const override;

	void ClearStorage() override;

	~ImageStorage();
private:
	void CreateTempDirectory();
	void ClearTempDirectory();
	std::string GenerateUniqueFilename() const;

	std::filesystem::path m_storageDir = std::filesystem::temp_directory_path();
};