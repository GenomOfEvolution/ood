#pragma once
#include <iostream>
#include "../PictureDraft/PictureDraft.h"

class IDesigner
{
public:
	virtual PictureDraft CreateDraft(std::istream& stream) = 0;
	virtual ~IDesigner() = default;
};