#pragma once

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <map>
#include <functional>
#include <string>
#include <memory>
#include <istream>

class QtGraphicsItemFactory
{
public:
    QtGraphicsItemFactory();

    std::unique_ptr<QGraphicsItem> CreateItem(const std::string& description);

private:
    std::unique_ptr<QGraphicsItem> CreateRectangle(std::istream& input);
    std::unique_ptr<QGraphicsItem> CreateTriangle(std::istream& input);
    std::unique_ptr<QGraphicsItem> CreateEllipse(std::istream& input);
    std::unique_ptr<QGraphicsItem> CreateImage(std::istream& input);

    using ItemCreator = std::function<std::unique_ptr<QGraphicsItem>(std::istream& input)>;
    using ItemCreators = std::map<std::string, ItemCreator>;

    const ItemCreators m_actionMap;
};