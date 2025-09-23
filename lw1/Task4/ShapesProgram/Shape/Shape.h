#pragma once
#include "ShapeStrategy/IShapeStrategy.h"
#include "../gfx/Color.h"
#include "../gfx/ICanvas.h"

#include <string>
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
		
		void Draw(gfx::ICanvas* canvas) const;
		std::string GetInfo() const;
		std::string GetType() const;

		void Move(double dx, double dy);
		gfx::Color GetColor() const;
		void SetColor(gfx::Color newColor);
		void SetStrategy(std::unique_ptr<IShapeStrategy>&& newStrategy);
	};

} // namespace shapes
