#pragma once
#include <functional>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>

#include "../ShapeStrategy/IShapeStrategy.h"
#include "../ShapeStrategy/EllipseStrategy.h"
#include "../ShapeStrategy/LineStrategy.h"
#include "../ShapeStrategy/RectangleStrategy.h"
#include "../ShapeStrategy/TextStrategy.h"
#include "../ShapeStrategy/TriangleStrategy.h"

using namespace shapes;

class ShapeStrategyFactory 
{
private:
    using Creator = std::function<std::unique_ptr<IShapeStrategy>(std::istream&)>;

    static const std::unordered_map<std::string, Creator>& GetCreators() 
    {
        static const std::unordered_map<std::string, Creator> creators = 
        {
            {"circle", [](std::istream& input) {
                double x, y, r;

                if (input >> x >> y >> r) 
                {
                    return std::make_unique<EllipseStrategy>(Point{ x, y }, r);
                }

                throw std::invalid_argument("Invalid circle parameters");
            }},
            {"rectangle", [](std::istream& input) {
                double left, top, width, height;

                if (input >> left >> top >> width >> height) 
                {
                    return std::make_unique<RectangleStrategy>(Point{ left, top }, width, height);
                }

                throw std::invalid_argument("Invalid rectangle parameters");
            }},
            {"triangle", [](std::istream& input) {
                double x1, y1, x2, y2, x3, y3;

                if (input >> x1 >> y1 >> x2 >> y2 >> x3 >> y3)
                {
                    return std::make_unique<TriangleStrategy>(Point{ x1, y1 }, Point{ x2, y2 }, Point{ x3, y3 });
                }

                throw std::invalid_argument("Invalid line parameters");
            }},
            {"line", [](std::istream& input) {
                double x1, y1, x2, y2;

                if (input >> x1 >> y1 >> x2 >> y2) 
                {
                    return std::make_unique<LineStrategy>(Point{ x1, y1 }, Point{ x2, y2 });
                }

                throw std::invalid_argument("Invalid line parameters");
            }},
            {"text", [](std::istream& input) {
                double left, top, size;
                std::string text;

                if (input >> left >> top >> size) 
                {
                    std::getline(input >> std::ws, text);
                    return std::make_unique<TextStrategy>(Point{ left, top }, size, text);
                }

                throw std::invalid_argument("Invalid text parameters");
            }}
        };

        return creators;
    }

public:
    static std::unique_ptr<IShapeStrategy> Create(const std::string& type, std::istream& input) 
    {
        const auto& creators = GetCreators();
        auto it = creators.find(type);
        if (it != creators.end())
        {
            return it->second(input);
        }
        throw std::invalid_argument("Unknown shape type: " + type);
    }
};