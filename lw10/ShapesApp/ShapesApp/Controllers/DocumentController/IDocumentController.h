#pragma once
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
	virtual void AddImageItem(const std::string& imagePath, int width = 0, int height = 0) = 0;
	virtual void RemoveItemAtIndex(size_t index) = 0;
};