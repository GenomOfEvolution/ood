#pragma once
#include "../../Models/Shared/HandleType.h"
#include "../../Models/Document/IDocument.h"
#include "../../Models/ImageStorage/IImageStorage.h"
#include <filesystem>
#include <vector>
#include <string>

class IDocumentController
{
public:
	virtual ~IDocumentController() = default;

	virtual bool WasDocumentSaved() const = 0;
	virtual void Save() = 0;
	virtual void SaveAs(const std::filesystem::path& path) = 0;
	virtual void Load(const std::filesystem::path& path) = 0;

	virtual bool CanUndo() const = 0;
	virtual void Undo() = 0;
	virtual bool CanRedo() const = 0;
	virtual void Redo() = 0;

	virtual void AddShape(const std::string& description) = 0;
	virtual void AddImageItem(const std::filesystem::path& imagePath, double width = 0, double height = 0) = 0;
	virtual void RemoveSelectedItems() = 0;
	virtual void Resize(HandleType type, double dx, double dy) = 0;
	virtual std::shared_ptr<const DocumentItem> GetItemAtIndex(size_t index) const = 0;
	virtual size_t GetItemsCount() const = 0;

	virtual std::vector<size_t> GetSelectedIndexes() const = 0;
	virtual std::shared_ptr<IImageStorage> GetImageStorage() const = 0;
};