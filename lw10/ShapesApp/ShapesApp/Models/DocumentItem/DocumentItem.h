#pragma once
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

private:
    std::variant<
        std::shared_ptr<IImage>,
        std::shared_ptr<IShape>
    > m_item;
};