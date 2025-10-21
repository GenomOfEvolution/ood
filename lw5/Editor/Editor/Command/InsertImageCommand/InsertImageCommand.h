#pragma once
#include "../AbstractCommand.h"
#include "../../DocumentItem/Image/IImage.h"
#include "../../DocumentItem/DocumentItem.h"
#include "../../Saver/ISaver.h"

#include <vector>
#include <memory>
#include <optional>
#include <filesystem>

class InsertImageCommand : public AbstractCommand
{
public:
	InsertImageCommand(
		std::vector<DocumentItem>& items,
		std::shared_ptr<IImage> image,
		std::optional<size_t> index,
		std::filesystem::path sourcePath,
		ISaver& saver);
	~InsertImageCommand();

private:
	void DoExecute() override;
	void DoUnexecute() override;
	void Destroy();

	std::vector<DocumentItem>& m_items;
	ISaver& m_saver;

	std::shared_ptr<IImage> m_image;
	std::optional<std::size_t> m_insertPos;
	size_t m_actualPosition = 0;
	std::filesystem::path m_imgSrcPath;

	bool m_shouldDelete = false;
};