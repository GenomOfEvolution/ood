#pragma once
#include "../Shapes/Shape.h"
#include <memory>
#include <string>

class IShapeFactory
{
public:
	virtual std::unique_ptr<Shape> CreateShape(const std::string& description) = 0;
	virtual ~IShapeFactory() = default;
};