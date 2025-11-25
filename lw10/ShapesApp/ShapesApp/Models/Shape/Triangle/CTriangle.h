#pragma once
#include "../IShape.h"
#include <qpoint.h>
#include <array>

class CTriangle : public IShape
{
public:
	CTriangle(const QPointF& p1, const QPointF& p2, const QPointF& p3);
	CTriangle(const std::array<QPointF, 3>& points);

	bool ContainsPoint(const QPointF& point) const override;
	QRectF GetBoundingBox() const override;
	void MoveBy(const QPointF& delta) override;
	void ApplyTransform(const QTransform& transform) override;

private:
	std::array<QPointF, 3> m_points;
};
