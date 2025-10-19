#pragma once
#include "ISaver.h"

class HtmlSaver : public ISaver
{
public:
	HtmlSaver();
	~HtmlSaver();

	void Save(const IDocument& document, const std::filesystem::path& path) override;
	void SaveTempImage(const IImage& image) override;
	void DeleteTempImage(const IImage& image) override;

private:
	void SetTempPath();

	static std::filesystem::path m_tempPath;
};