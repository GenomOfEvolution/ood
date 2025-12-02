#pragma once
#include "IDocumentController.h"
#include "../../Models/Document/IDocument.h"
#include "../../Models/History/ICommandExecutor.h"
#include <memory>

class DocumentController : public IDocumentController
{
public:
	DocumentController(
		std::shared_ptr<IDocument>&& document,
		std::shared_ptr<ICommandExecutor>&& history);

	bool WasDocumentSaved() const override;

	void Save() override;
	void SaveAs(const std::string& path) override;
	void Load(const std::string& path) override;

	bool CanUndo() const override;
	void Undo() override;
	bool CanRedo() const override;
	void Redo() override;

	void AddShape(const std::string& description) override;
	void AddImageItem(const std::string& imagePath, int width = 0, int height = 0) override;
	void RemoveItemAtIndex(size_t index) override;

private:
	std::shared_ptr<IDocument> m_document;
	std::shared_ptr<ICommandExecutor> m_history;

	bool m_wasDocumentSaved = false;
};