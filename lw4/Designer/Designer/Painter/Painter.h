#pragma once
#include "../PictureDraft/PictureDraft.h"
#include "../Canvas/ICanvas.h"

class Painter
{
public:
	void DrawPicture(const PictureDraft& draft, ICanvas& canvas) const;
};