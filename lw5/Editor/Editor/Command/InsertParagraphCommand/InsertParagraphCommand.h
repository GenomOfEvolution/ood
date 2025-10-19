#pragma once
#include "../AbstractCommand.h"
#include "../../DocumentItem/DocumentItem.h"
#include <vector>
#include <memory>
#include <optional>


class InsertParagraphCommand : public AbstractCommand
{
public:
	InsertParagraphCommand(
		std::vector<DocumentItem>& items,
		std::shared_ptr<IParagraph> newItem,
		std::optional<size_t> index);
private:
	void DoExecute() override;
	void DoUnexecute() override;

	std::vector<DocumentItem>& m_items;
	std::shared_ptr<IParagraph> m_newItem;
	std::optional<size_t> m_insertPos;
	size_t m_actualPosition = 0;
};