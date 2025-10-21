#include "ResizeImageCommand.h"

ResizeImageCommand::ResizeImageCommand(
	std::shared_ptr<IImage> image,
	int width, int height)
	: m_image(image)
	, m_newHeight(height)
	, m_newWidth(width)
{
}

void ResizeImageCommand::DoExecute()
{
	m_oldWidth = m_image->GetWidth();
	m_oldHeight = m_image->GetHeight();

	m_image->Resize(m_newWidth, m_newHeight);
}

void ResizeImageCommand::DoUnexecute()
{
	m_image->Resize(m_oldWidth, m_oldHeight);
}

bool ResizeImageCommand::CanMergeWith(const ICommand& other) const
{
	const ResizeImageCommand* otherCmd = dynamic_cast<const ResizeImageCommand*>(&other);
	if (otherCmd == nullptr)
		return false;

	return m_image == otherCmd->m_image;
}

void ResizeImageCommand::MergeWith(std::unique_ptr<ICommand> other)
{
	auto otherCmd = static_cast<ResizeImageCommand*>(other.get());

	m_newWidth = otherCmd->m_newWidth;
	m_newHeight = otherCmd->m_newHeight;
}
