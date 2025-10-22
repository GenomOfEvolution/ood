#pragma once
#include "../AbstractCommand.h"
#include "../../Document/IDocument.h"
#include "../../DocumentItem/DocumentItem.h"

class ListCommand : public AbstractCommand
{
public:
	ListCommand(IDocument& doc);

private:
	void DoExecute() override;
	void DoUnexecute() override;

	void PrintParagraph(const IParagraph& paragraph) const;
	void PrintImage(const IImage& image) const;

	IDocument& m_document;
};