#pragma once
#include "../../Shared/Geometry.h"
#include "../../Shared/ItemPreviewDTO.h"
#include <memory>

class IShape
{
public:
	virtual bool ContainsPoint(const Point& point) const = 0;
	virtual Rect GetBoundingBox() const = 0;
	virtual void MoveBy(const Point& delta) = 0;
	virtual void Resize(const Rect& newBoundingBox) = 0;
	virtual DocItemPreview GetPreview() const = 0;

	virtual ~IShape() = default;
};