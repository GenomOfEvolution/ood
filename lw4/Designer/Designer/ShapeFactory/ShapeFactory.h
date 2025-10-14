#pragma once
#include "IShapeFactory.h"
#include <map>
#include <functional>

class ShapeFactory : public IShapeFactory
{
public:
	std::unique_ptr<Shape> CreateShape(const std::string& description) override;

private:
	static inline const std::map<std::string, Color> m_stringToColor =
	{
		{ "red", Color::Red },
		{ "green", Color::Green },
		{ "blue", Color::Blue },
		{ "pink", Color::Pink },
		{ "yellow", Color::Yellow },
		{ "black", Color::Black },
	};

	static Color ParseColor(std::istream& line);
	static std::unique_ptr<Shape> CreateEllipse(std::istream& input);
	static std::unique_ptr<Shape> CreateRectangle(std::istream& input);
	static std::unique_ptr<Shape> CreateRegularPolygon(std::istream& input);
	static std::unique_ptr<Shape> CreateTriangle(std::istream& input);

	// TODO: назвать ShapeCreator
	using ShapeCreator = std::function<std::unique_ptr<Shape>(std::istream&)>;
	// TODO: ShapeCreators
	using ShapeCreators = std::map<std::string, ShapeCreator>;
	static inline const ShapeCreators m_actionMap =
	{
		{ "ellipse", CreateEllipse },
		{ "rectangle", CreateRectangle },
		{ "regular-polygon", CreateRegularPolygon },
		{ "triangle", CreateTriangle },
	};

	std::string FormAvailableFigures();
};
