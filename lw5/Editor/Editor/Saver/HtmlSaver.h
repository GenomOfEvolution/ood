#pragma once
#include "ISaver.h"
#include <fstream>
#include <map>

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
	void ClearTempFolder() const;

	void PrintParagraph(const IParagraph& paragraph, std::ofstream& htmlFile) const;
	void PrintImage(const IImage& image, std::ofstream& htmlFile) const;
	void PrintHtmlHead(const IDocument& document, std::ofstream& htmlFile) const;
	void PrintHtmlBody(const IDocument& document, std::ofstream& htmlFile) const;
	std::string HtmlEncode(const std::string& text) const;

	const static inline std::map<std::string, std::string> HTML_ENTITY_TABLE
	{
		{ "\"", "&quot;" },
		{ "'", "&apos;" },
		{ ">", "&gt;" },
		{ "<", "&lt;" },
		{ "&", "&amp;" },
		{ "\n", "<br>"}
	};

	std::filesystem::path m_tempPath;
};