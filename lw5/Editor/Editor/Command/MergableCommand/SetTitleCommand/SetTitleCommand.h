#pragma once
#include "../IMergableCommand.h"
#include <string>

class SetTitleCommand : public IMergableCommand
{
public:
	SetTitleCommand(std::string& title,	std::string newTitle);

	void DoExecute() override;
	void DoUnexecute() override;

	bool CanMergeWith(const ICommand& other) const override;
	void MergeWith(std::unique_ptr<ICommand> other) override;

private:
	std::string& m_title;
	std::string m_oldTitle;
	std::string m_newTitle;
};