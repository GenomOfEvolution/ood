#pragma once
#include "ISlide.h"

class Slide : public ISlide
{
public:
	Slide();
	IGroup& GetShapes() const override;
	void Draw(ICanvas& canvas) const override;

private:
	std::unique_ptr<IGroup> m_shapes;
};