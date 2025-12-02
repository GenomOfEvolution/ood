#pragma once
#include "ISerializer.h"
#include "../Document/IDocument.h"

class XmlSerializer : public ISerializer
{
public:
	XmlSerializer() = default;

	void Serialize(const std::string& path) override;
	void Deserialize(const std::string& path) override;
	void SetDocument(std::shared_ptr<IDocument> document) override;

private:
	std::shared_ptr<IDocument> m_document;
};