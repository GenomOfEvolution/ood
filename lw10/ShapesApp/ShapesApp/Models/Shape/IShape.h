#pragma once
#include <qpoint.h>
#include <qrect.h>
#include <memory>

class IGroup;

class IShape
{
public:
	virtual bool ContainsPoint(const QPointF& point) const = 0;
	virtual QRectF GetBoundingBox() const = 0;
	virtual void MoveBy(const QPointF& delta) = 0;
	virtual void ApplyTransform(const QTransform& transform) = 0;

	virtual ~IShape() = default;
};