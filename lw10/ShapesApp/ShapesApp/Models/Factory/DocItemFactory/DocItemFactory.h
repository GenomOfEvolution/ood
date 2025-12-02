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

	std::unique_ptr<DocumentItem> CreateItem(const std::string& description);

private:
	std::unique_ptr<DocumentItem> CreateTriangle(std::istream& input);
	std::unique_ptr<DocumentItem> CreateRectangle(std::istream& input);
	std::unique_ptr<DocumentItem> CreateEllipse(std::istream& input);
	std::unique_ptr<DocumentItem> CreateImage(std::istream& input);

	using ItemCreator = std::function<std::unique_ptr<DocumentItem>(std::istream& input)>;
	using ItemCreators = std::map<std::string, ItemCreator>;
	const ItemCreators m_actionMap;
};