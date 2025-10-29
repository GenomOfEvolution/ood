#pragma once
#include "IGroup.h"
#include "../Shape/Shape.h"
#include <vector>
#include <memory>
#include <iostream>

class GroupShape : public IGroup, public std::enable_shared_from_this<GroupShape>
{
public:
    std::shared_ptr<IGroup> GetGroup() override;
    std::shared_ptr<const IGroup> GetGroup() const override;

    size_t GetShapeCount() const override;
    std::shared_ptr<IShape> GetShapeAtIndex(size_t index) override;
    std::shared_ptr<const IShape> GetShapeAtIndex(size_t index) const override;
    
    void InsertShape(std::shared_ptr<IShape> shape, size_t index) override;
    void RemoveShapeAtIndex(size_t index) override;

private:
    std::vector<std::shared_ptr<IShape>> m_shapes;
};