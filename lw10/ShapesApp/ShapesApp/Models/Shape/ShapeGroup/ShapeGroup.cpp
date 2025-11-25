#include "ShapeGroup.h"
#include <algorithm>
#include <stdexcept>
#include <QTransform>
#include <QRectF>

bool ShapeGroup::ContainsPoint(const QPointF& point) const
{
    for (const auto& shape : m_shapes) 
    {
        if (shape->ContainsPoint(point)) 
        {
            return true;
        }
    }
    return false;
}

QRectF ShapeGroup::GetBoundingBox() const
{
    if (m_shapes.empty()) 
    {
        return QRectF(0, 0, 0, 0);
    }

    QRectF boundingBox = m_shapes[0]->GetBoundingBox();

    for (size_t i = 1; i < m_shapes.size(); ++i) 
    {
        boundingBox = boundingBox.united(m_shapes[i]->GetBoundingBox());
    }

    return boundingBox;
}

void ShapeGroup::MoveBy(const QPointF& delta)
{
    for (auto& shape : m_shapes) 
    {
        shape->MoveBy(delta);
    }
}

void ShapeGroup::ApplyTransform(const QTransform& transform)
{
    for (auto& shape : m_shapes) 
    {
        shape->ApplyTransform(transform);
    }
}

size_t ShapeGroup::GetShapeCount() const
{
    return m_shapes.size();
}

std::shared_ptr<IShape> ShapeGroup::GetShapeAtIndex(size_t index)
{
    if (index >= m_shapes.size()) {
        throw std::out_of_range("Index out of range");
    }
    return m_shapes[index];
}

std::shared_ptr<const IShape> ShapeGroup::GetShapeAtIndex(size_t index) const
{
    if (index >= m_shapes.size()) {
        throw std::out_of_range("Index out of range");
    }
    return m_shapes[index];
}

void ShapeGroup::InsertShape(std::shared_ptr<IShape> shape, size_t index)
{
    if (!shape) 
    {
        throw std::invalid_argument("Cannot insert null shape");
    }

    if (index > m_shapes.size()) 
    {
        throw std::out_of_range("Index out of range");
    }

    m_shapes.insert(m_shapes.begin() + index, shape);
}

void ShapeGroup::RemoveShapeAtIndex(size_t index)
{
    if (index >= m_shapes.size()) 
    {
        throw std::out_of_range("Index out of range");
    }

    m_shapes.erase(m_shapes.begin() + index);
}

void ShapeGroup::ClearGroup()
{
    m_shapes.clear();
}

bool ShapeGroup::ContainsShape(const IShape* shape) const
{
    if (!shape) return false;

    for (const auto& s : m_shapes) {
        if (s.get() == shape) {
            return true;
        }
    }
    return false;
}

size_t ShapeGroup::FindShapeIndex(const IShape* shape) const
{
    if (!shape) {
        throw std::invalid_argument("Shape cannot be null");
    }

    for (size_t i = 0; i < m_shapes.size(); ++i) {
        if (m_shapes[i].get() == shape) {
            return i;
        }
    }

    throw std::invalid_argument("Shape not found in group");
}