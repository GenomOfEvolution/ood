#include "ShapeFactory.h"
#include "../Shapes/Ellipse/Ellipse.h"
#include "../Shapes/Rectangle/Rectangle.h"
#include "../Shapes/RegularPolygon/RegularPolygon.h"
#include "../Shapes/Triangle/Triangle.h"

#include <iostream>
#include <sstream>

std::unique_ptr<Shape> ShapeFactory::CreateShape(const std::string& description)
{
    std::istringstream input(description);
    
    std::string shapeType;
    input >> shapeType;

    auto it = m_actionMap.find(shapeType);
    if (it == m_actionMap.end())
    {
        throw std::runtime_error("Unknown figure type! Available figures: " + FormAvailableFigures());
    }

    return it->second(input);
}

Color ShapeFactory::ParseColor(std::istream& line)
{
    std::string colorStr;

    line >> colorStr;
    auto it = m_stringToColor.find(colorStr);
    if (it == m_stringToColor.end())
    {
        throw std::runtime_error("Unknown color " + colorStr);
    }

    return it->second;
}

std::unique_ptr<Shape> ShapeFactory::CreateEllipse(std::istream& input)
{
    Color color = ParseColor(input);
    double x, y, horizontalRadius, verticalRadius;
    input >> x >> y >> horizontalRadius >> verticalRadius;

    if (input.bad() || input.fail())
    {
        throw std::invalid_argument("Invalid arguments for ellipse");
    }

    return std::make_unique<Ellipse>(
        color,
        Point{ x, y },
        horizontalRadius,
        verticalRadius
    );
}

std::unique_ptr<Shape> ShapeFactory::CreateRectangle(std::istream& input)
{
    Color color = ParseColor(input);
    double x, y, width, height;

    input >> x >> y >> width >> height;

    if (input.bad() || input.fail())
    {
        throw std::invalid_argument("Invalid arguments for rectangle (color, left top, width, height)");
    }

    return std::make_unique<Rectangle>(
        color,
        Point{ x, y },
        width,
        height
    );
}

std::unique_ptr<Shape> ShapeFactory::CreateRegularPolygon(std::istream& input)
{
    Color color = ParseColor(input);
    double radius, x, y;
    size_t vertices;
    input >> vertices >> radius >> x >> y;

    if (input.bad() || input.fail())
    {
        throw std::invalid_argument("Invalid arguments for regular polygon (color, vertices, radius, x, y)");
    }

    return std::make_unique<RegularPolygon>(
        color,
        vertices,
        radius,
        Point{ x, y }
    );
}

std::unique_ptr<Shape> ShapeFactory::CreateTriangle(std::istream& input)
{
    Color color = ParseColor(input);
    double x1, y1, x2, y2, x3, y3;
    input >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

    if (input.bad() || input.fail())
    {
        throw std::invalid_argument("Invalid arguments for triangle (color, x1, y1, x2, y2, x3, y3)");
    }

    return std::make_unique<Triangle>(
        color,
        Point{ x1, y1 },
        Point{ x2, y2 },
        Point{ x3, y3 }
    );
}

std::string ShapeFactory::FormAvailableFigures()
{
    std::string result;

    for (auto i : m_actionMap)
    {
        result += i.first + " ";
    }

    return result;
}
