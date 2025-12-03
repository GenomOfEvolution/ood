#include "DocItemFactory.h"
#include "../../DocumentItem/Shape/Ellipse/CEllipse.h"
#include "../../DocumentItem/Shape/Rectangle/CRectangle.h"
#include "../../DocumentItem/Shape/Triangle/CTriangle.h"
#include "../../DocumentItem/Image/CImage.h"
#include <sstream>

DocItemFactory::DocItemFactory()
{
}

std::unique_ptr<DocumentItem> DocItemFactory::CreateItem(const std::string& description)
{
    std::istringstream input(description);

    std::string commandType;
    input >> commandType;

    auto it = m_actionMap.find(commandType);
    if (it == m_actionMap.end())
    {
        throw std::runtime_error("Unknown command!");
    }

    return it->second(input);
}

std::unique_ptr<DocumentItem> DocItemFactory::CreateTriangle(std::istream& input)
{
    auto shape = std::make_shared<CTriangle>(Point{ 50, 0 }, Point{ 100, 50 }, Point{ 0, 50 });

    return std::make_unique<DocumentItem>(shape);
}

std::unique_ptr<DocumentItem> DocItemFactory::CreateRectangle(std::istream& input)
{
    auto shape = std::make_shared<CRectangle>(Point{ 0, 0 }, 100, 50);

    return std::make_unique<DocumentItem>(shape);
}

std::unique_ptr<DocumentItem> DocItemFactory::CreateEllipse(std::istream& input)
{
    auto shape = std::make_shared<CEllipse>(Point{ 50, 25 }, 25, 50);

    return std::make_unique<DocumentItem>(shape);
}

std::unique_ptr<DocumentItem> DocItemFactory::CreateImage(std::istream& input)
{
    std::string path;
    input >> path;

    auto image = std::make_shared<CImage>(path, Point{ 0, 0 }, 100, 50);

    return std::make_unique<DocumentItem>(std::static_pointer_cast<IImage>(image));
}
