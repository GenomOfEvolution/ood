#include "Client.h"

Client::Client(std::unique_ptr<ICanvas>&& canvas)
	: m_canvas(std::move(canvas))
{
}

void Client::AskForDraft(IDesigner& designer)
{
	m_draft = designer.CreateDraft(std::cin);
}

void Client::AskForPaintingDraft(const Painter& painter)
{
	painter.DrawPicture(m_draft, *m_canvas);
}
