#pragma once
#include "../../DocumentItem/DocumentItem.h"
#include "../../DocumentItem/Image/IImage.h"
#include "../../DocumentItem/Shape/IShape.h"

#include <map>
#include <functional>

class DocItemFactory
{
public:
	DocItemFactory();

	static std::unique_ptr<DocumentItem> CreateItem(const std::string& description);

private:
	static std::unique_ptr<DocumentItem> CreateTriangle(std::istream& input);
	static std::unique_ptr<DocumentItem> CreateRectangle(std::istream& input);
	static std::unique_ptr<DocumentItem> CreateEllipse(std::istream& input);
	static std::unique_ptr<DocumentItem> CreateImage(std::istream& input);

	using ItemCreator = std::function<std::unique_ptr<DocumentItem>(std::istream&)>;
	using ItemCreators = std::map<std::string, ItemCreator>;

	inline static const ItemCreators m_actionMap = 
	{
		{ "triangle",  [](std::istream& in) { return CreateTriangle(in); }},
		{ "rectangle", [](std::istream& in) { return CreateRectangle(in); }},
		{ "ellipse",   [](std::istream& in) { return CreateEllipse(in); }},
		{ "image",     [](std::istream& in) { return CreateImage(in); }}
	};
};