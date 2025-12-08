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

#include "../../Models/Shared/ItemPreviewDTO.h"

class QtGraphicsItemFactory
{
public:
    QtGraphicsItemFactory();

    std::unique_ptr<QGraphicsItem> CreateItem(const DocItemPreview& item);
    std::unique_ptr<QGraphicsItem> Clone(QGraphicsItem* item) const;

private:
    std::unique_ptr<QGraphicsItem> CloneRectangle(QGraphicsRectItem* source) const;
    std::unique_ptr<QGraphicsItem> CloneEllipse(QGraphicsEllipseItem* source) const;
    std::unique_ptr<QGraphicsItem> CloneTriangle(QGraphicsPolygonItem* source) const;
    std::unique_ptr<QGraphicsItem> CloneImage(QGraphicsPixmapItem* source) const;

    void CopyCommonProperties(const QGraphicsItem* source, QGraphicsItem* target) const;

    std::unique_ptr<QGraphicsItem> CreateRectangle(const DocItemPreview& input);
    std::unique_ptr<QGraphicsItem> CreateTriangle(const DocItemPreview& input);
    std::unique_ptr<QGraphicsItem> CreateEllipse(const DocItemPreview& input);
    std::unique_ptr<QGraphicsItem> CreateImage(const DocItemPreview& input);

    using ItemCreator = std::function<std::unique_ptr<QGraphicsItem>(const DocItemPreview& input)>;
    using ItemCreators = std::map<DocItemPreview::ItemType, ItemCreator>;

    const ItemCreators m_actionMap;
};