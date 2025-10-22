#pragma once
#include <filesystem>
#include "../Document/IDocument.h"
#include "../DocumentItem/Image/IImage.h"

class ISaver
{
public:
	virtual void Save(const IDocument& document, const std::filesystem::path& path) = 0;
	virtual std::filesystem::path SaveTempImage(const std::filesystem::path& srcPath) = 0;
	virtual void DeleteTempImage(const std::filesystem::path& srcPath) = 0;
};