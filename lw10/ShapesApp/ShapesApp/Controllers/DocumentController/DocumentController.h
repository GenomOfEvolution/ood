#pragma once
#include "IDocumentController.h"
#include "../../Models/Document/IDocument.h"
#include "../../Models/History/ICommandExecutor.h"
#include "../../Models/Factory/DocItemFactory/DocItemFactory.h"
#include "../../Models/ImageStorage/IImageStorage.h"
#include "../../Models/Selection/ISelection.h"
#include "../../Services/IDialogService.h"
#include <memory>
#include <QRectF>
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
		std::shared_ptr<IDialogService>&& dialogService,
		QObject* parent = nullptr);

	DocItemPreview ConvertToDTO(size_t index);

	bool WasDocumentSaved() const override;

	void Save() override;
	void SaveAs(const std::filesystem::path& path) override;
	void Load(const std::filesystem::path& path) override;

	void SaveWithDialog();

	bool CanUndo() const override;
	void Undo() override;
	bool CanRedo() const override;
	void Redo() override;

	void AddShape(const std::string& description) override;
	void AddImageItem(const std::filesystem::path& imagePath, double width = 0, double height = 0) override;
	void RemoveSelectedItems() override;

	void Resize(HandleType type, double dx, double dy) override;
	std::shared_ptr<const DocumentItem> GetItemAtIndex(size_t index) const override;
	size_t GetItemsCount() const override;

	std::vector<size_t> GetSelectedIndexes() const;

	std::shared_ptr<IImageStorage> GetImageStorage() const override;

	void handleMousePress(const QPointF& scenePos, Qt::KeyboardModifiers modifiers);
	void handleMouseMove(const QPointF& scenePos, Qt::KeyboardModifiers modifiers);
	void handleMouseRelease(const QPointF& scenePos, Qt::KeyboardModifiers modifiers);

signals:
	void documentChanged();
	void undoRedoAvailabilityChanged(bool canUndo, bool canRedo);
	void itemAdded(const DocItemPreview& preview);
	void deleteLastItem(size_t index);
	void itemsMoved(std::vector<size_t> indexes, double dx, double dy);
	void documentLoaded();
	void selectionChanged();
	void itemsResized(std::vector<QRectF> newBounds);

private:
	bool IsPointOverSelectedItem(const Point& point) const;

	std::shared_ptr<IDocument> m_document;
	std::shared_ptr<ICommandExecutor> m_history;
	std::shared_ptr<IImageStorage> m_storage;
	std::shared_ptr<ISelection> m_selection;
	std::shared_ptr<IDialogService> m_dialogService;

	DocItemFactory m_itemFactory;

	bool m_wasDocumentSaved = false;
	bool m_dragging = false;
	Point m_dragStartPoint;
};