#pragma once
#include "IParagraph.h"

class Paragraph : public IParagraph
{
public:
	Paragraph() = default;
	Paragraph(const std::string& text);

	std::string GetText() const;
	void SetText(const std::string& text);
	~Paragraph() = default;

private:
	std::string m_text;
};
