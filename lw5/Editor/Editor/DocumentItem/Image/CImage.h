#pragma once
#include "IImage.h"

class CImage : public IImage
{
public:
	CImage() = default;
	CImage(std::filesystem::path path, int width, int height);

	std::filesystem::path GetPath() const override;
	void SetPath(const std::filesystem::path& path) override;

	int GetWidth() const override;
	int GetHeight() const override;
	void Resize(int width, int height) override;

private:
	static inline const int MAX_IMAGE_SIZE = 10000;
	static inline const int MIN_IMAGE_SIZE = 1;

	bool IsCorrectSize(int size) const;

	std::filesystem::path m_path;
	int m_width = 0;
	int m_height = 0;
};