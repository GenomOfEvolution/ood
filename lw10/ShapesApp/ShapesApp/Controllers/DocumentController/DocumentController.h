#pragma once
#include "IDocumentController.h"
#include "../../Models/Document/IDocument.h"
#include "../../Models/History/ICommandExecutor.h"
#include "../../Models/Factory/DocItemFactory/DocItemFactory.h"
#include <memory>
#include <qobject.h>

class DocumentController : public QObject, public IDocumentController
{
	Q_OBJECT
public:
	DocumentController(
		std::shared_ptr<IDocument>&& document,
		std::shared_ptr<ICommandExecutor>&& history,
		QObject* parent = nullptr);

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

signals:
	void undoRedoAvailabilityChanged(bool canUndo, bool canRedo);
	void itemRemoved(int index);
	void itemAdded(const std::string& itemName);
	//void itemResized(int index, Rect boundingBox);

private:
	std::shared_ptr<IDocument> m_document;
	std::shared_ptr<ICommandExecutor> m_history;
	DocItemFactory m_itemFactory;

	bool m_wasDocumentSaved = false;
};