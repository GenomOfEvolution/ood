#pragma once
#include "IDocumentController.h"
#include "../../Models/Document/IDocument.h"
#include "../../Models/History/ICommandExecutor.h"
#include "../../Models/Factory/DocItemFactory/DocItemFactory.h"
#include "../../Models/ImageStorage/IImageStorage.h"
#include "../../Models/Selection/ISelection.h"
#include <memory>
#include <qobject.h>
#include <qpoint.h>

class DocumentController : public QObject, public IDocumentController
{
	Q_OBJECT
public:
	DocumentController(
		std::shared_ptr<IDocument>&& document,
		std::shared_ptr<ICommandExecutor>&& history,
		std::shared_ptr<IImageStorage>&& storage,
		std::shared_ptr<ISelection>&& selection,
		QObject* parent = nullptr);

	DocItemPreview ConvertToDTO(size_t index);

	bool WasDocumentSaved() const override;

	void Save() override;
	void SaveAs(const std::string& path) override;
	void Load(const std::string& path) override;

	bool CanUndo() const override;
	void Undo() override;
	bool CanRedo() const override;
	void Redo() override;

	void AddShape(const std::string& description) override;
	void AddImageItem(const std::string& imagePath, double width = 0, double height = 0) override;
	void RemoveSelectedItems() override;

	std::vector<size_t> GetSelectedIndexes() const;

	void handleMousePress(const QPointF& scenePos, Qt::KeyboardModifiers modifiers);
	void handleMouseMove(const QPointF& scenePos, Qt::KeyboardModifiers modifiers);
	void handleMouseRelease(const QPointF& scenePos, Qt::KeyboardModifiers modifiers);

signals:
	void undoRedoAvailabilityChanged(bool canUndo, bool canRedo);
	void itemRemoved(int index);
	void itemAdded(const DocItemPreview& preview);
	void itemsMoved(std::vector<size_t> indexes, double dx, double dy);
	void documentLoaded();
	void selectionChanged();

private:
	bool IsPointOverSelectedItem(const Point& point) const;

	std::shared_ptr<IDocument> m_document;
	std::shared_ptr<ICommandExecutor> m_history;
	std::shared_ptr<IImageStorage> m_storage;
	std::shared_ptr<ISelection> m_selection;

	DocItemFactory m_itemFactory;

	bool m_wasDocumentSaved = false;
	bool m_dragging = false;
	Point m_dragStartPoint;
};