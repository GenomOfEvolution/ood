#pragma once
#include <qpoint.h>

class ISelection
{
public:
	virtual ~ISelection() = default;

	virtual void SelectItem(const QPointF& point, bool ctrlPressed = false) = 0;
	virtual void ClearSelection() = 0;
};