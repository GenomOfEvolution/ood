#include "QtGraphicsItemFactory.h"
#include <QPixmap>
#include <qpen.h>
#include <QPolygonF>
#include <stdexcept>

QtGraphicsItemFactory::QtGraphicsItemFactory()
    : m_actionMap{
        {DocItemPreview::ItemType::Rectangle, [this](const DocItemPreview& in) { return CreateRectangle(in); }},
        {DocItemPreview::ItemType::Triangle,  [this](const DocItemPreview& in) { return CreateTriangle(in); }},
        {DocItemPreview::ItemType::Ellipse,   [this](const DocItemPreview& in) { return CreateEllipse(in); }},
        {DocItemPreview::ItemType::Image,     [this](const DocItemPreview& in) { return CreateImage(in); }}
    }
{
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateItem(const DocItemPreview& item)
{
    if (auto it = m_actionMap.find(item.m_type); it != m_actionMap.end())
    {
        return it->second(item);
    }

    throw std::runtime_error("Unknown graphical item type");
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateRectangle(const DocItemPreview& input)
{
    double 
        x = input.m_boundingBox.x,
        y = input.m_boundingBox.y,
        width = input.m_boundingBox.width,
        height = input.m_boundingBox.height;

    auto rect = std::make_unique<QGraphicsRectItem>(x, y, width, height);
    rect->setPen(QPen(Qt::black, 1));
    rect->setBrush(QBrush(Qt::red));
    return rect;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateTriangle(const DocItemPreview& input)
{
    double 
        x1 = input.m_points[0].x, y1 = input.m_points[0].y,
        x2 = input.m_points[1].x, y2 = input.m_points[1].y,
        x3 = input.m_points[2].x, y3 = input.m_points[2].y;

    QPolygonF polygon;
    polygon << QPointF(x1, y1) << QPointF(x2, y2) << QPointF(x3, y3);

    auto triangle = std::make_unique<QGraphicsPolygonItem>(polygon);
    triangle->setPen(QPen(Qt::black, 1)); 
    triangle->setBrush(QBrush(Qt::red));
    return triangle;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateEllipse(const DocItemPreview& input)
{
    double 
        x = input.m_boundingBox.x,
        y = input.m_boundingBox.y,
        width = input.m_boundingBox.width,
        height = input.m_boundingBox.height;

    auto ellipse = std::make_unique<QGraphicsEllipseItem>(x, y, width, height);
    ellipse->setPen(QPen(Qt::black, 1));
    ellipse->setBrush(QBrush(Qt::red));
    return ellipse;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateImage(const DocItemPreview& input)
{
    std::string path = input.m_imgPath;
    double 
        x = input.m_boundingBox.x,
        y = input.m_boundingBox.y,
        width = input.m_boundingBox.width,
        height = input.m_boundingBox.height;

    QPixmap pixmap(QString::fromStdString(path));
    if (pixmap.isNull()) 
    {
        throw std::runtime_error("Failed to load image: " + path);
    }

    pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    auto image = std::make_unique<QGraphicsPixmapItem>(pixmap);
    image->setPos(x, y);
    return image;
}