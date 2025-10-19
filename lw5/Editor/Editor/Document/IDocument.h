#pragma once
#include <memory>
#include <optional>
#include <string>
#include <filesystem>
#include "../DocumentItem/Image/IImage.h"
#include "../DocumentItem/Paragraph/IParagraph.h"
#include "../DocumentItem/DocumentItem.h"

class IDocument
{
public:
    virtual std::shared_ptr<IParagraph> InsertParagraph(
        const std::string& text,
        std::optional<size_t> position = std::nullopt) = 0;

    virtual std::shared_ptr<IImage> InsertImage(
        const std::filesystem::path& path,
        int width, int height,
        std::optional<size_t> position = std::nullopt) = 0;

    virtual size_t GetItemsCount() const = 0;

    virtual DocumentItem GetItem(size_t index) const = 0;
    virtual DocumentItem GetItem(size_t index) = 0;
    virtual void DeleteItem(size_t index) = 0;

    virtual std::string GetTitle() const = 0;
    virtual void SetTitle(const std::string& title) = 0;

    virtual bool CanUndo() const = 0;
    virtual void Undo() = 0;

    virtual bool CanRedo() const = 0;
    virtual void Redo() = 0;

    virtual void Save(const std::filesystem::path& path) const = 0;

    virtual ~IDocument() = default;
};