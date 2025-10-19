#pragma once
#include "Image/IImage.h"
#include "Paragraph/IParagraph.h"
#include <memory>
#include <variant>

class DocumentItem
{
public:
    explicit DocumentItem(std::shared_ptr<IImage> image)
        : m_item(std::move(image))
    {
    }

    explicit DocumentItem(std::shared_ptr<IParagraph> paragraph)
        : m_item(std::move(paragraph))
    {
    }

    std::shared_ptr<IImage> GetImage()
    {
        return std::holds_alternative<std::shared_ptr<IImage>>(m_item)
            ? std::get<std::shared_ptr<IImage>>(m_item)
            : nullptr;
    }

    std::shared_ptr<IParagraph> GetParagraph()
    {
        return std::holds_alternative<std::shared_ptr<IParagraph>>(m_item)
            ? std::get<std::shared_ptr<IParagraph>>(m_item)
            : nullptr;
    }

    std::shared_ptr<const IImage> GetImage() const
    {
        return std::holds_alternative<std::shared_ptr<IImage>>(m_item)
            ? std::get<std::shared_ptr<IImage>>(m_item)
            : nullptr;
    }

    std::shared_ptr<const IParagraph> GetParagraph() const
    {
        return std::holds_alternative<std::shared_ptr<IParagraph>>(m_item)
            ? std::get<std::shared_ptr<IParagraph>>(m_item)
            : nullptr;
    }

private:
    std::variant<
        std::shared_ptr<IImage>,
        std::shared_ptr<IParagraph>
    > m_item;
};