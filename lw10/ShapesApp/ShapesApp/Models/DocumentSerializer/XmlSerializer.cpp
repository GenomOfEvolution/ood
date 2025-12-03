#include "XmlSerializer.h"
#include <qfileinfo.h>
#include <qdir.h>
#include <iostream>

#include "../../Models/DocumentItem/Shape/Ellipse/CEllipse.h"
#include "../../Models/DocumentItem/Shape/Rectangle/CRectangle.h"
#include "../../Models/DocumentItem/Shape/Triangle/CTriangle.h"
#include "../../Models/DocumentItem/Image/CImage.h"

XmlSerializer::XmlSerializer(std::shared_ptr<IImageStorage> storage)
	: m_storage(storage)
{
}

void XmlSerializer::Serialize(const std::string& path)
{
	if (!m_document) 
	{
		throw std::runtime_error("Document is not set");
	}

    QFileInfo fileInfo(QString::fromStdString(path));
    QString docDir = fileInfo.absoluteDir().path();
    QString imagesDir = QDir(docDir).filePath("images");

    if (QDir(imagesDir).exists())
    {
        QDir dir(imagesDir);
        QStringList files = dir.entryList(QDir::Files);

        for (const QString& file : files)
        {
            if (!dir.remove(file))
            {
                throw std::runtime_error("Failed to remove existing file: " +
                    QDir::cleanPath(imagesDir + "/" + file).toStdString());
            }
        }
    }
    else if (!QDir().mkpath(imagesDir))
    {
        throw std::runtime_error("Failed to create images directory: " + imagesDir.toStdString());
    }

    m_storage->CopyAllImagesFromStorage(imagesDir.toStdString());
    
    SaveXmlDocument(path, docDir);
}

void XmlSerializer::Deserialize(const std::string& path)
{
    if (!m_document) 
    {
        throw std::runtime_error("Document is not set");
    }

    while (m_document->GetItemsCount() > 0) 
    {
        m_document->RemoveItemAtIndex(0);
    }

    m_storage->ClearStorage();

    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        throw std::runtime_error("Cannot open file for reading: " + path);
    }

    QXmlStreamReader xml(&file);
    QFileInfo fileInfo(QString::fromStdString(path));
    QString docDir = fileInfo.absoluteDir().path();

    ParseDocument(xml, docDir);
}

void XmlSerializer::SetDocument(std::shared_ptr<IDocument> document)
{
	m_document = document;
}

void XmlSerializer::SaveXmlDocument(const std::string& path, const QString& docDir)
{
    QFile file(QString::fromStdString(path));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
    {
        throw std::runtime_error("Cannot open file for writing: " + path);
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("document");
    xml.writeAttribute("version", "1.0");

    WriteItemsSection(xml, docDir);

    xml.writeEndElement();
    xml.writeEndDocument();

    if (xml.hasError() || file.error() != QFile::NoError) 
    {
        throw std::runtime_error("Error writing XML file: " + path);
    }
}

void XmlSerializer::WriteItemsSection(QXmlStreamWriter& xml, const QString& docDir)
{
    xml.writeStartElement("items");

    for (size_t i = 0; i < m_document->GetItemsCount(); ++i) 
    {
        auto item = m_document->GetItemAtIndex(i);

        if (auto image = item->GetImage()) 
        {
            auto img = std::dynamic_pointer_cast<CImage>(image);
            WriteImageItem(xml, *img);
        }
        else if (auto shape = item->GetShape()) 
        {
            if (auto ellipse = std::dynamic_pointer_cast<CEllipse>(shape)) 
            {
                WriteEllipseItem(xml, *ellipse);
            }
            else if (auto rect = std::dynamic_pointer_cast<CRectangle>(shape)) 
            {
                WriteRectangleItem(xml, *rect);
            }
            else if (auto triangle = std::dynamic_pointer_cast<CTriangle>(shape)) 
            {
                WriteTriangleItem(xml, *triangle);
            }
        }
    }

    xml.writeEndElement(); // items
}

void XmlSerializer::WriteImageItem(QXmlStreamWriter& xml, const IImage& item)
{
    xml.writeStartElement("item");

    xml.writeAttribute("type", "image");
    xml.writeTextElement("path", QString::fromStdString(item.GetPath()));

    WriteBoundingBox(xml, item.GetBoundingBox());

    xml.writeEndElement(); // item
}

void XmlSerializer::WriteEllipseItem(QXmlStreamWriter& xml, const IShape& item)
{
    const CEllipse* ellipse = dynamic_cast<const CEllipse*>(&item);

    Point center = ellipse->GetCenter();
    double vertRadius = ellipse->GetVertRadius();
    double horizRadius = ellipse->GetHorizRadius();

    xml.writeStartElement("item");
    xml.writeAttribute("type", "ellipse");

    xml.writeStartElement("geometry");
    xml.writeAttribute("centerX", QString::number(center.x, 'f', 2));
    xml.writeAttribute("centerY", QString::number(center.y, 'f', 2));
    xml.writeAttribute("vertRadius", QString::number(vertRadius, 'f', 2));
    xml.writeAttribute("horizRadius", QString::number(horizRadius, 'f', 2));
    xml.writeEndElement(); // geometry

    xml.writeEndElement(); // item
}

void XmlSerializer::WriteTriangleItem(QXmlStreamWriter& xml, const IShape& item)
{
    const CTriangle* triangle = dynamic_cast<const CTriangle*>(&item);
    auto points = triangle->GetPoints();

    xml.writeStartElement("item");
    xml.writeAttribute("type", "triangle");

    xml.writeStartElement("points");
    for (size_t i = 0; i < 3; ++i) 
    {
        xml.writeStartElement("point");
        xml.writeAttribute("x", QString::number(points[i].x, 'f', 2));
        xml.writeAttribute("y", QString::number(points[i].y, 'f', 2));
        xml.writeEndElement(); // point
    }
    xml.writeEndElement(); // points

    xml.writeEndElement(); // item
}

void XmlSerializer::WriteRectangleItem(QXmlStreamWriter& xml, const IShape& item)
{
    xml.writeStartElement("item");
    xml.writeAttribute("type", "rectangle");

    WriteBoundingBox(xml, item.GetBoundingBox());

    xml.writeEndElement(); // item
}

void XmlSerializer::WriteBoundingBox(QXmlStreamWriter& xml, const Rect& bbox)
{
    xml.writeStartElement("boundingBox");
    xml.writeAttribute("x", QString::number(bbox.x, 'f', 2));
    xml.writeAttribute("y", QString::number(bbox.y, 'f', 2));
    xml.writeAttribute("width", QString::number(bbox.width, 'f', 2));
    xml.writeAttribute("height", QString::number(bbox.height, 'f', 2));
    xml.writeEndElement(); // boundingBox
}

void XmlSerializer::ParseDocument(QXmlStreamReader& xml, const QString& docDir)
{
    while (!xml.atEnd() && !xml.hasError()) 
    {
        xml.readNext();

        if (xml.isStartElement() && xml.name() == QString("document")) 
        {
            while (xml.readNextStartElement()) 
            {
                if (xml.name() == QString("items")) 
                {
                    ParseItemsSection(xml, docDir);
                }
            }
        }
    }

    if (xml.hasError()) 
    {
        throw std::runtime_error("XML error: " + xml.errorString().toStdString());
    }
}

void XmlSerializer::ParseItemsSection(QXmlStreamReader& xml, const QString& docDir)
{
    while (xml.readNextStartElement()) 
    {
        if (xml.name() == QString("item")) 
        {
            QString type = xml.attributes().value("type").toString();

            if (type == "image") 
            {
                ParseImageItem(xml, docDir);
            }
            else if (type == "ellipse")
            {
                ParseEllipseItem(xml);
            }
            else if (type == "rectangle") 
            {
                ParseRectangleItem(xml);
            }
            else if (type == "triangle") 
            {
                ParseTriangleItem(xml);
            }
        }
        else
        {
            xml.skipCurrentElement();
        }
    }
}

void XmlSerializer::ParseImageItem(QXmlStreamReader& xml, const QString& documentDir)
{
    QString relativePath;
    Rect bbox;

    while (xml.readNextStartElement()) 
    {
        if (xml.name() == QString("path"))
        {
            relativePath = xml.readElementText();
        }
        else if (xml.name() == QString("boundingBox")) 
        {
            bbox = ReadBoundingBox(xml);
        }
        else 
        {
            xml.skipCurrentElement();
        }
    }

    if (relativePath.isEmpty()) 
    {
        throw std::runtime_error("Image path is missing in XML");
    }

    
    auto image = std::make_shared<CImage>(relativePath.toStdString(), Point{bbox.x, bbox.y}, bbox.width, bbox.height);

    m_document->AddItem(std::make_unique<DocumentItem>(std::static_pointer_cast<IImage>(image)));
}

void XmlSerializer::ParseEllipseItem(QXmlStreamReader& xml)
{
    Point center;
    double vertRadius = 0.0;
    double horizRadius = 0.0;
    bool hasGeometry = false;

    while (xml.readNextStartElement()) 
    {
        if (xml.name() == QString("geometry"))
        {
            auto attrs = xml.attributes();
            center.x = attrs.value("centerX").toDouble();
            center.y = attrs.value("centerY").toDouble();
            vertRadius = attrs.value("vertRadius").toDouble();
            horizRadius = attrs.value("horizRadius").toDouble();
            hasGeometry = true;
            xml.skipCurrentElement();
        }
        else 
        {
            xml.skipCurrentElement();
        }
    }

    if (!hasGeometry) 
    {
        throw std::runtime_error("Ellipse geometry data is missing");
    }

    auto ellipse = std::make_shared<CEllipse>(center, vertRadius, horizRadius);
    m_document->AddItem(std::make_unique<DocumentItem>(ellipse));
}

void XmlSerializer::ParseTriangleItem(QXmlStreamReader& xml)
{
    std::vector<Point> points;
    points.reserve(3);

    while (xml.readNextStartElement()) 
    {
        if (xml.name() == QString("points")) 
        {
            while (xml.readNextStartElement()) 
            {
                if (xml.name() == QString("point"))
                {
                    auto attrs = xml.attributes();
                    double x = attrs.value("x").toDouble();
                    double y = attrs.value("y").toDouble();
                    points.emplace_back(x, y);
                    xml.skipCurrentElement();
                }
                else 
                {
                    xml.skipCurrentElement();
                }
            }
        }
        else
        {
            xml.skipCurrentElement();
        }
    }

    if (points.size() != 3) 
    {
        throw std::runtime_error("Triangle must have exactly 3 points, got: " + std::to_string(points.size()));
    }

    auto triangle = std::make_shared<CTriangle>(points[0], points[1], points[2]);
    m_document->AddItem(std::make_unique<DocumentItem>(triangle));
}

void XmlSerializer::ParseRectangleItem(QXmlStreamReader& xml)
{
    Rect bbox;
    bool hasBBox = false;

    while (xml.readNextStartElement()) 
    {
        if (xml.name() == QString("boundingBox"))
        {
            bbox = ReadBoundingBox(xml);
            hasBBox = true;
        }
        else 
        {
            xml.skipCurrentElement();
        }
    }

    if (!hasBBox) 
    {
        throw std::runtime_error("Rectangle bounding box data is missing");
    }

    auto rect = std::make_shared<CRectangle>(bbox.GetLeftTop(), bbox.GetRightBottom());
    m_document->AddItem(std::make_unique<DocumentItem>(rect));
}

Rect XmlSerializer::ReadBoundingBox(QXmlStreamReader& xml)
{
    Rect bbox;
    auto attrs = xml.attributes();

    bbox.x = attrs.value("x").toDouble();
    bbox.y = attrs.value("y").toDouble();
    bbox.width = attrs.value("width").toDouble();
    bbox.height = attrs.value("height").toDouble();

    xml.skipCurrentElement();
    return bbox;
}