#pragma once
#include "../Shape.h"
#include "../ShapeTypes.h"

class RegularPolygon : public Shape
{
public:
	RegularPolygon(Color color, size_t vertexCount, double radius, Point center);
	
	void Draw(ICanvas& canvas) const override;
	double GetRadius() const { return m_radius; };
	size_t GetVertexCount() const { return m_vertexCount; };
	Point GetCenter() const { return m_center; };

private:
	static constexpr int MIN_VERTICES = 2;

	double m_radius;
	size_t m_vertexCount;
	Point m_center;
};