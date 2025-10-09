#define _USE_MATH_DEFINES
#include "RegularPolygon.h"
#include <iostream>
#include <vector>
#include <cmath>

RegularPolygon::RegularPolygon(Color color, size_t vertexCount, double radius, Point center)
	: Shape(color)
	, m_vertexCount(vertexCount)
	, m_center(center)
	, m_radius(radius)
{
	if (vertexCount < MIN_VERTICES)
	{
		throw std::invalid_argument("Minimal vertices for Regular Polygon is 2!");
	}
}

std::vector<Point> GetVerticies(Point center, double radius, size_t vertexCount)
{
	std::vector<Point> result;
	const auto vertexAngle = 2 * M_PI / static_cast<double>(vertexCount);

	result.reserve(vertexCount);
	for (std::size_t i = 0; i < vertexCount; ++i)
	{
		result.push_back(
		{
			center.x + radius * std::cos(vertexAngle * static_cast<double>(i)),
			center.y + radius * std::sin(vertexAngle * static_cast<double>(i)),
		});
	}

	return result;
}

void RegularPolygon::Draw(ICanvas& canvas) const
{
	canvas.SetColor(GetColor());

	std::vector<Point> vertices = GetVerticies(m_center, m_radius, m_vertexCount);
	for (int i = 0; i < vertices.size() - 1; i++)
	{
		canvas.DrawLine(vertices[i], vertices[i + 1]);
	}
	canvas.DrawLine(vertices[vertices.size() - 1], vertices[0]);
}

