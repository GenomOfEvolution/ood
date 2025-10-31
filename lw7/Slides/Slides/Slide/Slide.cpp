#include "Slide.h"
#include "../ShapeGroup/GroupShape.h"

Slide::Slide()
{
    m_shapes = std::make_unique<GroupShape>();
}

IGroup& Slide::GetShapes() const
{
    return *m_shapes;
}

void Slide::Draw(ICanvas& canvas) const
{
    for (size_t i = 0; i < m_shapes->GetShapeCount(); i++)
    {
        m_shapes->GetShapeAtIndex(i)->Draw(canvas);
    }
}