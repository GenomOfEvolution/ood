#pragma once
#include "../IShape.h"
#include <qpoint.h>

class CEllipse : public IShape
{
public:
	CEllipse(const QPointF& center, float verticalRadius, float horizontalRadius);

	bool ContainsPoint(const QPointF& point) const override;
	QRectF GetBoundingBox() const override;
	void MoveBy(const QPointF& delta) override;
	void ApplyTransform(const QTransform& transform) override;

private:
	QPointF m_center;
	float m_vertRadius, m_horizRadius;
};