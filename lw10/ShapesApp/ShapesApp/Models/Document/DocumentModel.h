#pragma once
#include "IDocument.h"
#include "../History/IHistory.h"
#include "../DocumentSerializer/ISerializer.h"
#include <vector>

class DocumentModel : public IDocument
{
public:
	DocumentModel(
		std::shared_ptr<IHistory>&& history,
		std::shared_ptr<ISerializer>&& saver,
		const Rect& fieldArea);

	void Save() override;
	void SaveAs(const std::string& path) override;
	void Load(const std::string& path) override;
	std::string GetSavePath() const override;

	Rect GetBounds() const override;
	Point MoveItemsBy(const std::vector<size_t> indexes, const Point& delta) override;

	void AddItem(std::unique_ptr<DocumentItem>&& item) override;
	std::shared_ptr<DocumentItem> GetItemAtIndex(size_t index) override;
	std::shared_ptr<const DocumentItem> GetItemAtIndex(size_t index) const override;
	void RemoveItemAtIndex(size_t index) override;

	size_t GetItemsCount() const override;

	bool CanUndo() const override;
	void Undo() override;

	bool CanRedo() const override;
	void Redo() override;

private:
	Point ConstrainPoint(const Point& p) const;
	Rect CalculateGroupBoundingBox(const std::vector<size_t>& indexes) const;

	std::shared_ptr<IHistory> m_history;
	std::shared_ptr<ISerializer> m_saver;
	std::vector<std::shared_ptr<DocumentItem>> m_items;
	Rect m_fieldArea;

	std::string m_savePath;
};