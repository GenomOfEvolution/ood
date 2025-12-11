#include "QtGraphicsItemFactory.h"
#include <QPixmap>
#include <qpen.h>
#include <QPolygonF>
#include <stdexcept>
#include <qfileinfo.h>
#include <qdir.h>

QtGraphicsItemFactory::QtGraphicsItemFactory(std::shared_ptr<IImageStorage> storage)
    : m_actionMap
    {
        { DocItemPreview::ItemType::Rectangle, [this](const DocItemPreview& in) { return CreateRectangle(in); }},
        { DocItemPreview::ItemType::Triangle,  [this](const DocItemPreview& in) { return CreateTriangle(in); }},
        { DocItemPreview::ItemType::Ellipse,   [this](const DocItemPreview& in) { return CreateEllipse(in); }},
        { DocItemPreview::ItemType::Image,     [this](const DocItemPreview& in) { return CreateImage(in); }}
    }
    , m_storage(storage)
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

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::Clone(QGraphicsItem* item) const
{
    if (!item) 
    {
        return nullptr;
    }

    if (auto rectItem = dynamic_cast<QGraphicsRectItem*>(item)) 
    {
        return CloneRectangle(rectItem);
    }
    else if (auto ellipseItem = dynamic_cast<QGraphicsEllipseItem*>(item)) 
    {
        return CloneEllipse(ellipseItem);
    }
    else if (auto polygonItem = dynamic_cast<QGraphicsPolygonItem*>(item)) 
    {
        return CloneTriangle(polygonItem);
    }
    else if (auto pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item)) 
    {
        return CloneImage(pixmapItem);
    }

    return nullptr;
}

void QtGraphicsItemFactory::CopyCommonProperties(const QGraphicsItem* source, QGraphicsItem* target) const
{
    if (!source || !target) 
        return;

    target->setPos(source->pos());
    target->setRotation(source->rotation());
    target->setScale(source->scale());
    target->setZValue(source->zValue());
    target->setTransform(source->transform());
    target->setFlags(source->flags());

    for (int role = Qt::UserRole; role < Qt::UserRole + 10; ++role) {
        target->setData(role, source->data(role));
    }
}

QString QtGraphicsItemFactory::GetImageTruePath(const QString& relativePath)
{
    QFileInfo fileInfo(relativePath);
    QString fileName = fileInfo.fileName();

    std::filesystem::path storagePath = m_storage->GetTempDirPath();

    return QString::fromStdString(storagePath.string()) + QDir::separator() + fileName;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CloneRectangle(QGraphicsRectItem* source) const
{
    auto newItem = std::make_unique<QGraphicsRectItem>(source->rect());
    newItem->setBrush(source->brush());
    newItem->setPen(source->pen());
    CopyCommonProperties(source, newItem.get());
    return newItem;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CloneEllipse(QGraphicsEllipseItem* source) const
{
    auto newItem = std::make_unique<QGraphicsEllipseItem>(source->rect());
    newItem->setBrush(source->brush());
    newItem->setPen(source->pen());
    CopyCommonProperties(source, newItem.get());
    return newItem;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CloneTriangle(QGraphicsPolygonItem* source) const
{
    auto newItem = std::make_unique<QGraphicsPolygonItem>(source->polygon());
    newItem->setBrush(source->brush());
    newItem->setPen(source->pen());
    CopyCommonProperties(source, newItem.get());
    return newItem;
}

std::unique_ptr<QGraphicsItem> QtGraphicsItemFactory::CloneImage(QGraphicsPixmapItem* source) const
{
    auto newItem = std::make_unique<QGraphicsPixmapItem>(source->pixmap());
    newItem->setOffset(source->offset());
    newItem->setTransformationMode(source->transformationMode());

    // Для изображений также копируем альфа-канал
    newItem->setOpacity(source->opacity());

    CopyCommonProperties(source, newItem.get());
    return newItem;
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
    QString path = QString::fromLocal8Bit(input.m_imgPath.c_str());
    double
        x = input.m_boundingBox.x,
        y = input.m_boundingBox.y,
        targetWidth = input.m_boundingBox.width,
        targetHeight = input.m_boundingBox.height;

    QPixmap originalPixmap(GetImageTruePath(path));
    if (originalPixmap.isNull())
    {
        throw std::runtime_error("Failed to load image: ");
    }

    int absWidth = qAbs(static_cast<int>(targetWidth));
    int absHeight = qAbs(static_cast<int>(targetHeight));

    QPixmap scaled = originalPixmap.scaled(absWidth, absHeight,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);

    auto imageItem = std::make_unique<QGraphicsPixmapItem>(scaled);
    double posX = targetWidth < 0 ? x + targetWidth : x;
    double posY = targetHeight < 0 ? y + targetHeight : y;

    imageItem->setPos(posX, posY);

    // Сохраняем данные
    imageItem->setData(OriginalPixmapRole, originalPixmap);
    imageItem->setData(TargetSizeRole, QSizeF(absWidth, absHeight));
    imageItem->setData(OriginalSizeRole, QSizeF(originalPixmap.width(), originalPixmap.height()));

    return imageItem;
}