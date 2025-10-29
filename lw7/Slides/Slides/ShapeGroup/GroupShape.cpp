#include "GroupShape.h"

std::shared_ptr<IGroup> GroupShape::GetGroup()
{
    return shared_from_this();
}

std::shared_ptr<const IGroup> GroupShape::GetGroup() const
{
    return shared_from_this();
}

size_t GroupShape::GetShapeCount() const
{
    return m_shapes.size();
}

std::shared_ptr<IShape> GroupShape::GetShapeAtIndex(size_t index)
{
    if (index >= m_shapes.size())
    {
        throw std::out_of_range("Index out of range");
    }

    return m_shapes.at(index);
}

std::shared_ptr<const IShape> GroupShape::GetShapeAtIndex(size_t index) const
{
    if (index >= m_shapes.size())
    {
        throw std::out_of_range("Index out of range");
    }

    return m_shapes.at(index);
}

void GroupShape::InsertShape(std::shared_ptr<IShape> shape, size_t index)
{
    if (index > m_shapes.size()) 
    {
        throw std::out_of_range("Index out of range");
    }

    m_shapes.insert(m_shapes.begin() + index, shape);
}

void GroupShape::RemoveShapeAtIndex(size_t index)
{
    if (index >= m_shapes.size()) 
    {
        throw std::out_of_range("Index out of range");
    }

    m_shapes.erase(m_shapes.begin() + index);
}
