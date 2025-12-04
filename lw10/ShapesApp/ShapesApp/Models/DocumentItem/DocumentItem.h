#pragma once
#include "../Shared/Geometry.h"
#include "../Shared/HandleType.h"
#include "Image/IImage.h"
#include "Shape/IShape.h"
#include <memory>
#include <variant>

class DocumentItem
{
public:
    explicit DocumentItem(std::shared_ptr<IImage> image)
        : m_item(std::move(image))
    {    }

    explicit DocumentItem(std::shared_ptr<IShape> shape)
        : m_item(std::move(shape))
    {    }

    std::shared_ptr<IImage> GetImage()
    {
        return std::holds_alternative<std::shared_ptr<IImage>>(m_item)
            ? std::get<std::shared_ptr<IImage>>(m_item)
            : nullptr;
    }

    std::shared_ptr<IShape> GetShape()
    {
        return std::holds_alternative<std::shared_ptr<IShape>>(m_item)
            ? std::get<std::shared_ptr<IShape>>(m_item)
            : nullptr;
    }

    std::shared_ptr<const IImage> GetImage() const
    {
        return std::holds_alternative<std::shared_ptr<IImage>>(m_item)
            ? std::get<std::shared_ptr<IImage>>(m_item)
            : nullptr;
    }

    std::shared_ptr<const IShape> GetShape() const
    {
        return std::holds_alternative<std::shared_ptr<IShape>>(m_item)
            ? std::get<std::shared_ptr<IShape>>(m_item)
            : nullptr;
    }

    DocItemPreview GetPreview() const 
    {
        if (std::holds_alternative<std::shared_ptr<IImage>>(m_item))
            return std::get<std::shared_ptr<IImage>>(m_item)->GetPreview();
        else
            return std::get<std::shared_ptr<IShape>>(m_item)->GetPreview();
    }

    bool ContainsPoint(const Point& point) const 
    {
        if (std::holds_alternative<std::shared_ptr<IImage>>(m_item))
            return std::get<std::shared_ptr<IImage>>(m_item)->ContainsPoint(point);
        else
            return std::get<std::shared_ptr<IShape>>(m_item)->ContainsPoint(point);
    }

    void MoveBy(const Point& delta) 
    {
        if (std::holds_alternative<std::shared_ptr<IImage>>(m_item))
            std::get<std::shared_ptr<IImage>>(m_item)->MoveBy(delta);
        else
            std::get<std::shared_ptr<IShape>>(m_item)->MoveBy(delta);
    }

    Rect GetBoundingBox() const
    {
        if (std::holds_alternative<std::shared_ptr<IImage>>(m_item))
            return std::get<std::shared_ptr<IImage>>(m_item)->GetBoundingBox();
        else
            return std::get<std::shared_ptr<IShape>>(m_item)->GetBoundingBox();
    }

    void Resize(HandleType type, double dx, double dy)
    {
        if (std::holds_alternative<std::shared_ptr<IImage>>(m_item))
        {
            auto item = std::get<std::shared_ptr<IImage>>(m_item);
            auto bbox = GetResizedBoundingBox(item->GetBoundingBox(), type, dx, dy);
            item->Resize(bbox);
        }
        else
        {
            auto item = std::get<std::shared_ptr<IShape>>(m_item);
            auto oldBbox = item->GetBoundingBox();
            auto bbox = GetResizedBoundingBox(oldBbox, type, dx, dy);
            item->Resize(bbox);
        }
    }

private:
    Rect GetResizedBoundingBox(const Rect& bbox, HandleType type, double dx, double dy)
    {
        Rect newRect = bbox;

        switch(type) 
        {
            case HandleType::TopLeft:
                newRect.x += dx;
                newRect.y += dy;
                newRect.width -= dx;
                newRect.height -= dy;
                break;
            case HandleType::TopRight:
                newRect.y += dy;
                newRect.width += dx;
                newRect.height -= dy;
                break;
            case HandleType::BottomLeft:
                newRect.x += dx;
                newRect.width -= dx;
                newRect.height += dy;
                break;
            case HandleType::BottomRight:
                newRect.width += dx;
                newRect.height += dy;
                break;
        }

        return newRect;
    }

    std::variant<
        std::shared_ptr<IImage>,
        std::shared_ptr<IShape>
    > m_item;
};