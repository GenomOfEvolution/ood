#pragma once
#include "IGroup.h"
#include "../Shape/Shape.h"
#include "../Style/GroupStyles/GroupFillStyle.h"
#include "../Style/GroupStyles/GroupOutlineStyle.h"

#include <vector>
#include <memory>
#include <iostream>

class GroupShape : public IGroup, public std::enable_shared_from_this<GroupShape>
{
public:
    GroupShape();

    std::shared_ptr<IGroup> GetGroup() override;
    std::shared_ptr<const IGroup> GetGroup() const override;

    std::optional<RectD> GetFrame() const override;
    void SetFrame(const RectD& rect) override;

    std::shared_ptr<IOutlineStyle> GetOutlineStyle() override;
    std::shared_ptr<const IOutlineStyle> GetOutlineStyle() const override;

    std::shared_ptr<IStyle> GetFillStyle() override;
    std::shared_ptr<const IStyle> GetFillStyle() const override;

    size_t GetShapeCount() const override;
    std::shared_ptr<IShape> GetShapeAtIndex(size_t index) override;
    std::shared_ptr<const IShape> GetShapeAtIndex(size_t index) const override;
    
    void InsertShape(std::shared_ptr<IShape> shape, size_t index) override;
    void RemoveShapeAtIndex(size_t index) override;

private:
    std::vector<std::shared_ptr<IShape>> m_shapes;
    std::shared_ptr<GroupFillStyle> m_fillStyle;
    std::shared_ptr<GroupOutlineStyle> m_outlineStyle;
};