#pragma once
#include <string>
#include <memory>
#include "../DocumentItem/DocumentItem.h"
#include "../Shared/Geometry.h"

class IDocument
{
public:
	virtual void Save() = 0;
	virtual void SaveAs(const std::string& path) = 0;
	virtual void Load(const std::string& path) = 0;
	virtual std::string GetSavePath() const = 0;

	virtual Rect GetBounds() const = 0;
	virtual Point MoveItemsBy(const std::vector<size_t> indexes, const Point& delta) = 0;
	virtual std::vector<Rect> ResizeItemsBy(const std::vector<size_t> indexes, const Point& delta, HandleType resizePoint) = 0;

	virtual void AddItem(std::unique_ptr<DocumentItem>&& item) = 0;
	virtual void InsertItemAtIndex(std::unique_ptr<DocumentItem>&& item, size_t index) = 0;
	virtual std::shared_ptr<DocumentItem> GetItemAtIndex(size_t index) = 0;
	virtual std::shared_ptr<const DocumentItem> GetItemAtIndex(size_t index) const = 0;
	virtual void RemoveItemAtIndex(size_t index) = 0;

	virtual size_t GetItemsCount() const = 0;

	virtual bool CanUndo() const = 0;
	virtual void Undo() = 0;

	virtual bool CanRedo() const = 0;
	virtual void Redo() = 0;

	virtual ~IDocument() = default;
};