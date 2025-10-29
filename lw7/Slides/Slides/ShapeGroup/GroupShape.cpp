#include "GroupShape.h"

GroupShape::GroupShape()
{
    auto fillStyleEnumerator = [this](std::function<void(IStyle&)> const& callback) 
    {
        for (auto&& shape : m_shapes)
        {
            if (auto style = shape->GetFillStyle())
            {
                callback(*style);
            }
        }
    };

    auto outlineStyleEnumerator = [this](std::function<void(IOutlineStyle&)> const& callback)
    {
        for (auto&& shape : m_shapes)
        {
            if (auto style = shape->GetOutlineStyle())
            {
                callback(*style);
            }
        }
    };

    m_fillStyle = std::make_shared<GroupFillStyle>(std::move(fillStyleEnumerator));
    m_outlineStyle = std::make_shared<GroupOutlineStyle>(std::move(outlineStyleEnumerator));
}

std::shared_ptr<IGroup> GroupShape::GetGroup()
{
    return shared_from_this();
}

std::shared_ptr<const IGroup> GroupShape::GetGroup() const
{
    return shared_from_this();
}

std::optional<RectD> GroupShape::GetFrame() const
{
    if (m_shapes.empty())
    {
        return std::nullopt;
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    bool hasValidFrame = false;

    for (auto&& shape : m_shapes)
    {
        auto shapeFrame = shape->GetFrame();
        if (shapeFrame.has_value())
        {
            hasValidFrame = true;
            const auto& frame = shapeFrame.value();

            minX = std::min(minX, frame.left);
            minY = std::min(minY, frame.top);
            maxX = std::max(maxX, frame.left + frame.width);
            maxY = std::max(maxY, frame.top + frame.height);
        }
    }

    if (!hasValidFrame)
    {
        return std::nullopt;
    }

    return RectD
    {
        minX,           
        minY,           
        maxX - minX,    
        maxY - minY     
    };
}

void GroupShape::SetFrame(const RectD& rect)
{
    auto groupFrame = GetFrame();
    if (!groupFrame.has_value())
    {
        return;
    }

    const auto& currentFrame = groupFrame.value();

    double scaleX = (currentFrame.width != 0) ? rect.width / currentFrame.width : 1.0;
    double scaleY = (currentFrame.height != 0) ? rect.height / currentFrame.height : 1.0;

    for (auto&& shape : m_shapes)
    {
        auto shapeFrame = shape->GetFrame();
        if (shapeFrame.has_value())
        {
            const auto& frame = shapeFrame.value();

            double newX = rect.left + (frame.left - currentFrame.left) * scaleX;
            double newY = rect.top + (frame.top - currentFrame.top) * scaleY;
            double newWidth = frame.width * scaleX;
            double newHeight = frame.height * scaleY;

            shape->SetFrame(RectD{ newX, newY, newWidth, newHeight });
        }
    }
}

std::shared_ptr<IOutlineStyle> GroupShape::GetOutlineStyle()
{
    return m_outlineStyle;
}

std::shared_ptr<const IOutlineStyle> GroupShape::GetOutlineStyle() const
{
    return m_outlineStyle;
}

std::shared_ptr<IStyle> GroupShape::GetFillStyle()
{
    return m_fillStyle;
}

std::shared_ptr<const IStyle> GroupShape::GetFillStyle() const
{
    return m_fillStyle;
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

void GroupShape::Draw(ICanvas& canvas) const
{
    for (size_t i = 0; i < m_shapes.size(); i++)
    {
        m_shapes[i]->Draw(canvas);
    }
}
