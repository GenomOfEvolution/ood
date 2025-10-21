#pragma once
#include "../IMergableCommand.h"
#include "../../../DocumentItem/Paragraph/IParagraph.h"

class ReplaceTextCommand : public IMergableCommand
{
public:
	ReplaceTextCommand(std::shared_ptr<IParagraph> paragraph, std::string newText);

	void DoExecute() override;
	void DoUnexecute() override;

	bool CanMergeWith(const ICommand& other) const override;
	void MergeWith(std::unique_ptr<ICommand> other) override;

private:
	std::shared_ptr<IParagraph> m_paragraph;
	std::string m_newText;
	std::string m_oldText;

	bool m_hasOldText = false;
};