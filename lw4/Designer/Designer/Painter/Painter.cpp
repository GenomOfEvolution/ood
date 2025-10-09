#include "Painter.h"

void Painter::DrawPicture(const PictureDraft& draft, ICanvas& canvas) const
{
	for (const auto& fig : draft)
	{
		fig->Draw(canvas);
	}
}
