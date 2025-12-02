#pragma once
#include "../IShape.h"

class CEllipse : public IShape
{
public:
	CEllipse(const Point& center, double verticalRadius, double horizontalRadius);

	bool ContainsPoint(const Point& point) const override;
	Rect GetBoundingBox() const override;
	void MoveBy(const Point& delta) override;
	void Resize(const Rect& newBoundingBox) override;

private:
	Point m_center;
	double m_vertRadius, m_horizRadius;
};