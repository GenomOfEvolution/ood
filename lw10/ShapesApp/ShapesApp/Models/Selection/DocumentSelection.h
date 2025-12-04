#pragma once
#include "ISelection.h"
#include "../Document/IDocument.h"

class DocumentSelection : public ISelection
{
public:
	DocumentSelection() = default;

	void SelectItem(const Point& point, bool additive = false) override;
	void ClearSelection() override;
	std::vector<size_t> GetSelectedIndexes() const override;
	void SetDocument(std::shared_ptr<IDocument> doc) override;

private:
	std::shared_ptr<IDocument> m_document;
	std::vector<size_t> m_selectedItems;
};