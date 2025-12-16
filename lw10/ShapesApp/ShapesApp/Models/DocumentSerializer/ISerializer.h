#pragma once
#include "../Document/IDocument.h"
#include <filesystem>

class ISerializer
{
public:
	virtual void Serialize(const std::filesystem::path& path) = 0;
	virtual void Deserialize(const std::filesystem::path& path) = 0;
	virtual void SetDocument(std::shared_ptr<IDocument> document) = 0;

	virtual ~ISerializer() = default;
};