#pragma once
#include "../Canvas/ICanvas.h"
#include "../PictureDraft/PictureDraft.h"
#include "../Designer/IDesigner.h"
#include "../Painter/Painter.h"
#include <memory>

class Client
{
public:
	Client(std::unique_ptr<ICanvas>&& canvas);
	void AskForDraft(IDesigner& designer);
	void AskForPaintingDraft(const Painter& painter);

private:
	PictureDraft m_draft;
	std::unique_ptr<ICanvas> m_canvas;
};
