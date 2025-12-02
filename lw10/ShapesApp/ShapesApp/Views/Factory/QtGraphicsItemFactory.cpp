#include "QtGraphicsItemFactory.h"
#include <QPixmap>
#include <qpen.h>
#include <QPolygonF>
#include <stdexcept>
#include <sstream>

QtGraphicsItemFactory::QtGraphicsItemFactory()
    : m_actionMap{
        {"rectangle", [this](std::istream& in) { return CreateRectangle(in); }},
        {"triangle",  [this](std::istream& in) { return CreateTriangle(in); }},
        {"ellipse",   [this](std::istream& in) { return CreateEllipse(in); }},
        {"image",     [this](std::istream& in) { return CreateImage(in); }}
    }
{
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateItem(const std::string& description)
{
    std::istringstream input(description);
    std::string commandType;
    input >> commandType;

    if (auto it = m_actionMap.find(commandType); it != m_actionMap.end()) {
        return it->second(input);
    }

    throw std::runtime_error("Unknown graphical item type: " + commandType);
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateRectangle(std::istream& input)
{
    double x = 0, y = 0, width = 100, height = 50;
    input >> x >> y >> width >> height;

    auto rect = std::make_unique<QGraphicsRectItem>(x, y, width, height);
    rect->setPen(QPen(Qt::black, 1));
    rect->setBrush(QBrush(Qt::red));
    return rect;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateTriangle(std::istream& input)
{
    double x1 = 50, y1 = 0, x2 = 100, y2 = 50, x3 = 0, y3 = 50;
    input >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

    QPolygonF polygon;
    polygon << QPointF(x1, y1) << QPointF(x2, y2) << QPointF(x3, y3);

    auto triangle = std::make_unique<QGraphicsPolygonItem>(polygon);
    triangle->setPen(QPen(Qt::black, 1)); 
    triangle->setBrush(QBrush(Qt::red));
    return triangle;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateEllipse(std::istream& input)
{
    double x = 0, y = 0, width = 100, height = 50;
    input >> x >> y >> width >> height;

    auto ellipse = std::make_unique<QGraphicsEllipseItem>(x, y, width, height);
    ellipse->setPen(QPen(Qt::black, 1));
    ellipse->setBrush(QBrush(Qt::red));
    return ellipse;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CreateImage(std::istream& input)
{
    std::string path;
    double x = 0, y = 0, width = 100, height = 50;
    input >> path >> x >> y >> width >> height;

    QPixmap pixmap(QString::fromStdString(path));
    if (pixmap.isNull()) {
        throw std::runtime_error("Failed to load image: " + path);
    }

    pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    auto image = std::make_unique<QGraphicsPixmapItem>(pixmap);
    image->setPos(x, y);
    return image;
}