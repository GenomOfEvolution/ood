#pragma once
#include "../Common/CommonTypes.h"
#include "../Style/IOutlineStyle.h"
#include "../Style/IStyle.h"
#include "../Canvas/IDrawable.h"
#include <memory>
#include <optional>

class IGroup;

class IShape : public IDrawable
{
public:
    virtual ~IShape() = default;

    virtual std::optional<RectD> GetFrame() const = 0;
    virtual void SetFrame(const RectD& rect) = 0;

    virtual std::shared_ptr<IOutlineStyle> GetOutlineStyle() = 0;
    virtual std::shared_ptr<const IOutlineStyle> GetOutlineStyle() const = 0;

    virtual std::shared_ptr<IStyle> GetFillStyle() = 0;
    virtual std::shared_ptr<const IStyle> GetFillStyle() const = 0;

    virtual std::shared_ptr<IGroup> GetGroup() = 0;
    virtual std::shared_ptr<const IGroup> GetGroup() const = 0;
};