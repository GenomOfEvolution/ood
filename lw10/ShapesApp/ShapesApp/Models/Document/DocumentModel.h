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
		std::shared_ptr<ISerializer>&& saver);

	void Save() override;
	void SaveAs(const std::string& path) override;
	void Load(const std::string& path) override;
	std::string GetSavePath() const override;

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
	std::shared_ptr<IHistory> m_history;
	std::shared_ptr<ISerializer> m_saver;
	std::vector<std::shared_ptr<DocumentItem>> m_items;

	std::string m_savePath;
};