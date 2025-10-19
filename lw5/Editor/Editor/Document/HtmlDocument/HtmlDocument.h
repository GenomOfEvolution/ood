#pragma once
#include "../IDocument.h"
#include "../../History/IHistory.h"
#include "../../Saver/ISaver.h"
#include "../../DocumentItem/DocumentItem.h"
#include <vector>
#include <string>

class HtmlDocument : public IDocument
{
public:
	HtmlDocument(std::unique_ptr<IHistory> history, std::unique_ptr<ISaver> saver);
	~HtmlDocument() = default;

	std::shared_ptr<IParagraph> InsertParagraph(
		const std::string& text,
		std::optional<size_t> position = std::nullopt) override;

	std::shared_ptr<IImage> InsertImage(
		const std::filesystem::path& path,
		int width, int height,
		std::optional<size_t> position = std::nullopt) override;

	size_t GetItemsCount() const override;

	DocumentItem GetItem(size_t index) const override;
	DocumentItem GetItem(size_t index) override;
	void DeleteItem(size_t index) override;

	std::string GetTitle() const override;
	void SetTitle(const std::string& title) override;

	bool CanUndo() const override;
	void Undo() override;

	bool CanRedo() const override;
	void Redo() override;

	void Save(const std::filesystem::path& path) const override;

private:
	std::unique_ptr<IHistory> m_history;
	std::unique_ptr<ISaver> m_saver;

	std::vector<DocumentItem> m_items;
	std::string m_title;
};
