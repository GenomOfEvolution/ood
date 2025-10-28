#include "SaveCommand.h"

SaveCommand::SaveCommand(IDocument& doc, std::filesystem::path path)
	: m_document(doc)
	, m_path(path)
{
}

void SaveCommand::DoExecute()
{
	m_document.Save(m_path);
}