#pragma once
#include "IDesigner.h"
#include "../ShapeFactory/IShapeFactory.h"

class Designer : public IDesigner
{
public:
	Designer(IShapeFactory& shapeFactory);
	PictureDraft CreateDraft(std::istream& stream);

private:
	IShapeFactory& m_factory;
};