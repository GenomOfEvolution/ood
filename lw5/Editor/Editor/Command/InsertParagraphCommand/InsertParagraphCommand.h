#pragma once
#include "../AbstractUndoableCommand.h"
#include "../../Document/IDocument.h"
#include <optional>
#include <string>


class InsertParagraphCommand : public AbstractUndoableCommand
{
public:
	InsertParagraphCommand(
		IDocument& doc,
		std::optional<size_t> index,
		std::string text);
private:
	void DoExecute() override;
	void DoUnexecute() override;

	IDocument& m_document;
	std::string m_text;
	std::optional<size_t> m_insertPos;
	size_t m_actualPosition = 0;
};