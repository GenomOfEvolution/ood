#pragma once
#include "../AbstractUndoableCommand.h"
#include "../../Document/IDocument.h"
#include "../../ImageStorage/IImageStorage.h"

class AddImageCommand : public AbstractUndoableCommand
{
public:
	AddImageCommand(
		IDocument& doc,
		IImageStorage& storage,
		const std::filesystem::path& srcPath, double width, double height);
	~AddImageCommand();

private:
	void DoExecute() override;
	void DoUnexecute() override;
	void Destroy();

	IDocument& m_document;
	IImageStorage& m_storage;
	std::filesystem::path m_srcPath;
	std::string m_tempPath;
	double m_width, m_height;
	std::shared_ptr<IImage> m_image;

	bool m_shouldDelete = false;
	size_t m_insertPos = 0;
};