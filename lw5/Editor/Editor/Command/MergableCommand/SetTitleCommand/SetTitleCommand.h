#pragma once
#include "../IMergableCommand.h"
#include "../../../Document/IDocument.h"
#include <string>

class SetTitleCommand : public IMergableCommand
{
public:
	SetTitleCommand(IDocument& doc,	std::string newTitle);

	void DoExecute() override;
	void DoUnexecute() override;

	bool CanMergeWith(const ICommand& other) const override;
	void MergeWith(std::unique_ptr<ICommand> other) override;

private:
	IDocument& m_document;
	std::string m_oldTitle;
	std::string m_newTitle;
};