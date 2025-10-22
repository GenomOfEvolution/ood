#include "SetTitleCommand.h"

SetTitleCommand::SetTitleCommand(IDocument& doc, std::string newTitle)
	: m_document(doc)
	, m_newTitle(std::move(newTitle))
{
}

void SetTitleCommand::DoExecute()
{
	m_oldTitle = m_document.GetTitle();
	m_document.SetTitle(m_newTitle);
}

void SetTitleCommand::DoUnexecute()
{
	m_document.SetTitle(m_oldTitle);
}

bool SetTitleCommand::CanMergeWith(const ICommand& other) const
{
	auto otherCmd = dynamic_cast<const SetTitleCommand*>(&other);
	return otherCmd != nullptr && &m_document == &otherCmd->m_document;
}

void SetTitleCommand::MergeWith(std::unique_ptr<ICommand> other)
{
	auto otherTitleCmd = static_cast<SetTitleCommand*>(other.get());
	m_newTitle = otherTitleCmd->m_newTitle;
}
