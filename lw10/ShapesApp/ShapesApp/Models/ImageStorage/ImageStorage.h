#pragma once
#include "IImageStorage.h"

class ImageStorage : public IImageStorage
{
public:
	ImageStorage();
	ImageStorage(const std::string& tempDirPath);

	void CopyAllImagesToStorage(const std::string& srcPath) override;
	std::string SaveImage(const std::string& srcPath) override;
	void DeleteTempImage(const std::string& srcPath) override;
	void CopyAllImagesFromStorage(const std::string& copyTo) override;

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