#pragma once
#include "../../Document/IDocument.h"
#include "../AbstractCommand.h"
#include <filesystem>

class SaveCommand : public AbstractCommand
{
public:
	SaveCommand(IDocument& doc, std::filesystem::path path);

private:
	void DoExecute() override;

	IDocument& m_document;
	std::filesystem::path m_path;
};