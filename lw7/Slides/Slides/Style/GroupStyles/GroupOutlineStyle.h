#pragma once
#include "../IOutlineStyle.h"
#include "GroupStyleBase.h"

class GroupOutlineStyle : public GroupStyleBase<IOutlineStyle>
{
public:
    using GroupStyleBase::GroupStyleBase;

    std::optional<double> GetThickness() const override
    {
        std::function<std::optional<double>(IOutlineStyle&)> getter =
            [](IOutlineStyle& style) { return style.GetThickness(); };

        return GetAggregatedProperty<double>(getter);
    }

    void SetThickness(double thickness) override
    {
        ForEachStyle([thickness](IOutlineStyle& style) 
        {
            style.SetThickness(thickness);
        });
    }
};