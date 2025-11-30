#pragma once
#include "../History/IHistory.h"

class IDocument : public IHistory
{
public:
	virtual void Save() = 0;

	virtual ~IDocument() = default;
};