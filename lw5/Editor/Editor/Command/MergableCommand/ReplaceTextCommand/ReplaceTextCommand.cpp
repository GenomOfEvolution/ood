#include "ReplaceTextCommand.h"

ReplaceTextCommand::ReplaceTextCommand(std::shared_ptr<IParagraph> paragraph, std::string newText)
	: m_paragraph(paragraph)
	, m_newText(newText)
{
}

void ReplaceTextCommand::DoExecute()
{
	if (!m_hasOldText)
	{
		m_oldText = m_paragraph->GetText();
		m_hasOldText = true;
	}

	m_paragraph->SetText(m_newText);
}

void ReplaceTextCommand::DoUnexecute()
{
	if (m_hasOldText)
	{
		m_paragraph->SetText(m_oldText);
	}
}

bool ReplaceTextCommand::CanMergeWith(const ICommand& other) const
{
	const ReplaceTextCommand* otherCmd = dynamic_cast<const ReplaceTextCommand*>(&other);
	if (otherCmd == nullptr)
		return false;

	return m_paragraph == otherCmd->m_paragraph;
}

void ReplaceTextCommand::MergeWith(std::unique_ptr<ICommand> other)
{
	auto otherCmd = static_cast<ReplaceTextCommand*>(other.get());
	m_newText = otherCmd->m_newText;
}
