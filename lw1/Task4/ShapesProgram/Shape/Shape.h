#pragma once
#include "ShapeStrategy/IShapeStrategy.h"
#include "../gfx/Color.h"

#include <memory>

namespace shapes
{
	struct Point
	{
		double x;
		double y;

		Point(double x, double y) : x(x), y(y) {}
	};


	class Shape
	{
	private:
		std::unique_ptr<IShapeStrategy> m_shapeStrategy;
		gfx::Color m_color;

	public:
		Shape(std::unique_ptr<IShapeStrategy>&& shapeStrategy) : 
			m_shapeStrategy(std::move(shapeStrategy)) 
		{}
		


	};

} // namespace shapes
