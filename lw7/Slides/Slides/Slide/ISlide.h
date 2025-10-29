#pragma once
#include "../Canvas/IDrawable.h"
#include "../ShapeGroup/IGroup.h"

class ISlide : public IDrawable
{
public:
	virtual ~ISlide() = default;
	virtual IGroup& GetShapes() const = 0;
};