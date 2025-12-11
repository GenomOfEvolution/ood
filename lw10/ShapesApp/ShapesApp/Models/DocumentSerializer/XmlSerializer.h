#pragma once
#include "ISerializer.h"
#include "../ImageStorage/IImageStorage.h"
#include "../Document/IDocument.h"

#include <qxmlstream.h>

class XmlSerializer : public ISerializer
{
public:
	XmlSerializer(std::shared_ptr<IImageStorage> storage);

	void Serialize(const std::filesystem::path& path) override;
	void Deserialize(const std::filesystem::path& path) override;
	void SetDocument(std::shared_ptr<IDocument> document) override;

private:
	// Serialize
	void SaveXmlDocument(const std::string& path, const QString& docDir);
	void WriteItemsSection(QXmlStreamWriter& xml, const QString& docDir);

	void WriteImageItem(QXmlStreamWriter& xml, const IImage& item);
	void WriteEllipseItem(QXmlStreamWriter& xml, const IShape& item);
	void WriteTriangleItem(QXmlStreamWriter& xml, const IShape& item);
	void WriteRectangleItem(QXmlStreamWriter& xml, const IShape& item);

	void WriteBoundingBox(QXmlStreamWriter& xml, const Rect& bbox);

	// Deserialize
	void ParseDocument(QXmlStreamReader& xml, const QString& docDir);
	void ParseItemsSection(QXmlStreamReader& xml, const QString& docDir);
	void ParseImageItem(QXmlStreamReader& xml, const QString& docDir);
	void ParseEllipseItem(QXmlStreamReader& xml);
	void ParseTriangleItem(QXmlStreamReader& xml);	
	void ParseRectangleItem(QXmlStreamReader& xml);

	Rect ReadBoundingBox(QXmlStreamReader& xml);

	std::shared_ptr<IDocument> m_document;
	std::shared_ptr<IImageStorage> m_storage;
};