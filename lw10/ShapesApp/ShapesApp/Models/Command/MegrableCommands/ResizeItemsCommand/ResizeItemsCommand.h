#pragma once
#include "../IMergableCommand.h"
#include "../../../Document/IDocument.h"
#include "../../../Selection/ISelection.h"

using ItemsResizedCallback = std::function<void(const std::vector<Rect>&)>;

class ResizeItemsCommand : public IMergableCommand
{
public:
	ResizeItemsCommand(
		IDocument& doc,
		ISelection& selection,
		HandleType type,
		Point delta,
		ItemsResizedCallback onItemsResized
	);

	bool CanMergeWith(const ICommand& other) const override;
	void MergeWith(std::unique_ptr<ICommand> other) override;

private:
	void DoExecute() override;
	void DoUnexecute() override;

	IDocument& m_document;
	ISelection& m_selection;

	std::vector<size_t> m_selectedIndexes;

	std::vector<Rect> m_oldBboxes;
	std::vector<Rect> m_newBBoxes;

	HandleType m_type;
	Point m_delta;
	ItemsResizedCallback m_onItemsResized;
};