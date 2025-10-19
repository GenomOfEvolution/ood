#include "InsertImageCommand.h"

InsertImageCommand::InsertImageCommand(
	std::vector<DocumentItem>& items,
	std::shared_ptr<IImage> image,
	std::optional<size_t> index,
	std::filesystem::path path)
	: m_items(items)
	, m_image(std::move(image))
	, m_insertPos(index)
{

}

InsertImageCommand::~InsertImageCommand()
{
}

void InsertImageCommand::DoExecute()
{
	
}

void InsertImageCommand::DoUnexecute()
{
}
