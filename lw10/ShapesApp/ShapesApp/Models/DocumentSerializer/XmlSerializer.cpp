#include "XmlSerializer.h"

void XmlSerializer::Serialize(const std::string& path)
{
}

void XmlSerializer::Deserialize(const std::string& path)
{
}

void XmlSerializer::SetDocument(std::shared_ptr<IDocument> document)
{
	m_document = document;
}
