#pragma once
#include "../IShape.h"

class CRectangle : public IShape
{
public:
	CRectangle(const QPointF& leftTop, float width, float height);
	CRectangle(const QPointF& leftTop, const QPoint& rightBottom);

	bool ContainsPoint(const QPointF& point) const override;
	QRectF GetBoundingBox() const override;
	void MoveBy(const QPointF& delta) override;
	void ApplyTransform(const QTransform& transform) override;

private:
	QRectF m_rect;
};