#pragma once
#include "../AbstractCommand.h"
#include "../../Document/IDocument.h"
#include "../../Saver/ISaver.h"

#include <optional>
#include <filesystem>
#include <memory>

class InsertImageCommand : public AbstractCommand
{
public:
	InsertImageCommand(
		IDocument& doc,
		ISaver& saver,
		std::optional<size_t> index,
		int width, 
		int height,
		std::filesystem::path sourcePath);
	~InsertImageCommand();

private:
	void DoExecute() override;
	void DoUnexecute() override;
	void Destroy();

	IDocument& m_document;
	ISaver& m_saver;

	std::optional<std::size_t> m_insertPos;
	size_t m_actualPosition = 0;

	int m_width, m_height;
	std::filesystem::path m_imgSrcPath;
	std::filesystem::path m_tempPath;
	std::shared_ptr<IImage> m_image;

	bool m_shouldDelete = false;
};