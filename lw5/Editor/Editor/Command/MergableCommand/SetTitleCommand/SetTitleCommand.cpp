#include "SetTitleCommand.h"

SetTitleCommand::SetTitleCommand(std::string& title, std::string newTitle)
	: m_oldTitle(title)
	, m_title(title)
	, m_newTitle(std::move(newTitle))
{
}

void SetTitleCommand::DoExecute()
{
	m_oldTitle = m_title;
	m_title = m_newTitle;
}

void SetTitleCommand::DoUnexecute()
{
	m_title = m_oldTitle;
}

bool SetTitleCommand::CanMergeWith(const ICommand& other) const
{
	auto otherCmd = dynamic_cast<const SetTitleCommand*>(&other);
	return otherCmd != nullptr && &m_title == &otherCmd->m_title;
}

void SetTitleCommand::MergeWith(std::unique_ptr<ICommand> other)
{
	auto otherTitleCmd = static_cast<SetTitleCommand*>(other.get());
	m_newTitle = otherTitleCmd->m_newTitle;
}
