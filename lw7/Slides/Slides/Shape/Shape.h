#pragma once
#include "IShape.h"
#include "../Style/ShapeStyles/ShapeFillStyle.h"
#include "../Style/ShapeStyles/ShapeLineStyle.h"

class Shape : public IShape 
{
public:
    Shape() 
        : m_fillStyle(std::move(std::make_shared<ShapeFillStyle>()))
        , m_outlineStyle(std::move(std::make_shared<ShapeLineStyle>()))
    {};

    std::shared_ptr<IGroup> GetGroup() override { return nullptr; }
    std::shared_ptr<const IGroup> GetGroup() const override { return nullptr; }

    std::shared_ptr<IStyle> GetFillStyle() override { return m_fillStyle; }
    std::shared_ptr<IStyle const> GetFillStyle() const override { return m_fillStyle; }

    std::shared_ptr<IOutlineStyle> GetOutlineStyle() override { return m_outlineStyle; }
    std::shared_ptr<IOutlineStyle const> GetOutlineStyle() const override { return m_outlineStyle; }

    virtual ~Shape() = default;

protected:
    std::optional<RGBAColor> GetOutlineColor() const
    {
        std::optional<RGBAColor> result = std::nullopt;
        if (m_outlineStyle->IsEnabled())
        {
            result = m_outlineStyle->GetColor();
        }
        return result;
    }

    std::optional<double> GetOutlineThickness() const
    {
        std::optional<double> result = std::nullopt;
        if (m_outlineStyle->IsEnabled())
        {
            result = m_outlineStyle->GetThickness();
        }
        return result;
    }

    std::optional<RGBAColor> GetFillColor() const
    {
        std::optional<RGBAColor> result = std::nullopt;
        if (m_fillStyle->IsEnabled())
        {
            result = m_fillStyle->GetColor();
        }
        return result;
    }

private:
    std::shared_ptr<IOutlineStyle> m_outlineStyle;
    std::shared_ptr<IStyle> m_fillStyle;
};