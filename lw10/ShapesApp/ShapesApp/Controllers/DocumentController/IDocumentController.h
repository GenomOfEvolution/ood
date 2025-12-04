#pragma once
#include "../../Models/Shared/HandleType.h"
#include <vector>
#include <string>

class IDocumentController
{
public:
	virtual ~IDocumentController() = default;

	virtual bool WasDocumentSaved() const = 0;
	virtual void Save() = 0;
	virtual void SaveAs(const std::string& path) = 0;
	virtual void Load(const std::string& path) = 0;

	virtual bool CanUndo() const = 0;
	virtual void Undo() = 0;
	virtual bool CanRedo() const = 0;
	virtual void Redo() = 0;

	virtual void AddShape(const std::string& description) = 0;
	virtual void AddImageItem(const std::string& imagePath, double width = 0, double height = 0) = 0;
	virtual void RemoveSelectedItems() = 0;
	virtual void Resize(HandleType type, double dx, double dy) = 0;

	virtual std::vector<size_t> GetSelectedIndexes() const = 0;
};