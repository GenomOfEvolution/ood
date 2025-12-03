#pragma once
#include "../IShape.h"
#include <array>

class CTriangle : public IShape
{
public:
	CTriangle(const Point& p1, const Point& p2, const Point& p3);
	CTriangle(const std::array<Point, 3>& points);

	std::array<Point, 3> GetPoints() const;

	bool ContainsPoint(const Point& point) const override;
	Rect GetBoundingBox() const override;
	void MoveBy(const Point& delta) override;
	void Resize(const Rect& newBoundingBox) override;

private:
	std::array<Point, 3> m_points;
};
