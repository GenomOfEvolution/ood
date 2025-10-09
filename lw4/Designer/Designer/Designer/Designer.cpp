#include "Designer.h"

Designer::Designer(IShapeFactory& shapeFactory)
	: m_factory(shapeFactory)
{
}

PictureDraft Designer::CreateDraft(std::istream& stream)
{
	PictureDraft draft;
	std::string description;

	std::cout << "Write empty line for exit";
	std::cout << "\n> ";
	while (std::getline(stream, description))
	{
		if (description.empty())
		{
			break;
		}

		try
		{
			draft.AddShape(m_factory.CreateShape(description));
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << "\n";
		}

		std::cout << "> ";
	}

	return draft;
}
