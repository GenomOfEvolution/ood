#pragma once
#include "../IShape.h"

class CEllipse : public IShape
{
public:
	CEllipse(const Point& center, double verticalRadius, double horizontalRadius);

	Point GetCenter() const;
	double GetVertRadius() const;
	double GetHorizRadius() const;

	bool ContainsPoint(const Point& point) const override;
	Rect GetBoundingBox() const override;
	void MoveBy(const Point& delta) override;
	void Resize(const Rect& newBoundingBox) override;
	DocItemPreview GetPreview() const override;

private:
	Point m_center;
	double m_vertRadius, m_horizRadius;
};