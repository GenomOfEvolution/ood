#pragma once
#include "../Shape/IShape.h"

class IGroup : public IShape 
{
public:
    virtual size_t GetShapeCount() const = 0;
    virtual std::shared_ptr<IShape> GetShapeAtIndex(size_t index) = 0;
    virtual std::shared_ptr<const IShape> GetShapeAtIndex(size_t index) const = 0;
    virtual void InsertShape(std::shared_ptr<IShape> shape, size_t index) = 0;
    virtual void RemoveShapeAtIndex(size_t index) = 0;

    virtual ~IGroup() = default;
};