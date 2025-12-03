#pragma once
#include "../IShape.h"

class CRectangle : public IShape
{
public:
	CRectangle(const Point& leftTop, double width, double height);
	CRectangle(const Point& leftTop, const Point& rightBottom);

	bool ContainsPoint(const Point& point) const override;
	Rect GetBoundingBox() const override;
	void MoveBy(const Point& delta) override;
	void Resize(const Rect& newBoundingBox) override;
	DocItemPreview GetPreview() const override;

private:
	Rect m_rect;
};