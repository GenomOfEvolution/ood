#pragma once
#include "../AbstractUndoableCommand.h"
#include "../../Document/IDocument.h"
#include "../../ImageStorage/IImageStorage.h"
#include "../../Selection/ISelection.h"

using ImageAddedCallback = std::function<void()>;
using ImageRemovedCallback = std::function<void()>;

class AddImageCommand : public AbstractUndoableCommand
{
public:
	AddImageCommand(
		IDocument& doc,
		ISelection& selection,
		IImageStorage& storage,
		const std::filesystem::path& srcPath,
		double width, double height,
		ImageAddedCallback onImageAdded = nullptr,
		ImageRemovedCallback onImageRemoved = nullptr
	);
	~AddImageCommand();

private:
	void DoExecute() override;
	void DoUnexecute() override;
	void Destroy();

	IDocument& m_document;
	IImageStorage& m_storage;
	ISelection& m_selection;

	ImageAddedCallback m_onImageAdded;
	ImageRemovedCallback m_onImageRemoved;

	std::filesystem::path m_srcPath;
	std::string m_tempPath;
	double m_width, m_height;
	std::shared_ptr<IImage> m_image;

	bool m_shouldDelete = false;
	size_t m_insertPos = 0;
};