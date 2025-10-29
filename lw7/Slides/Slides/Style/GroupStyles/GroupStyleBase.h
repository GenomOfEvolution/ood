#pragma once
#include <functional>
#include <optional>
#include <vector>

template <typename StyleInterface>
class GroupStyleBase : public StyleInterface
{
public:
    using StyleEnumerator = std::function<void(std::function<void(StyleInterface&)>)>;

    explicit GroupStyleBase(StyleEnumerator&& enumerator)
        : m_enumerator(std::move(enumerator))
    {
    }

    std::optional<bool> IsEnabled() const override
    {
        return GetAggregatedProperty(std::function<std::optional<bool>(StyleInterface&)>
        (
            [](StyleInterface& style) { return style.IsEnabled(); }
        ));
    }

    void Enable() override
    {
        ForEachStyle([](StyleInterface& style) { style.Enable(); });
    }

    void Disable() override
    {
        ForEachStyle([](StyleInterface& style) { style.Disable(); });
    }

    std::optional<RGBAColor> GetColor() const override
    {
        return GetAggregatedProperty(std::function<std::optional<RGBAColor>(StyleInterface&)>
        (
            [](StyleInterface& style) { return style.GetColor(); }
        ));
    }

    void SetColor(RGBAColor color) override
    {
        ForEachStyle([color](StyleInterface& style) { style.SetColor(color); });
    }

protected:
    template<typename T>
    std::optional<T> GetAggregatedProperty(std::function<std::optional<T>(StyleInterface&)> getter) const
    {
        std::optional<T> value;
        bool first = true;

        const_cast<GroupStyleBase*>(this)->m_enumerator([&](StyleInterface& style) 
        {
            auto currentValue = getter(style);
            if (first) 
            {
                value = currentValue;
                first = false;
            }
            else if (value != currentValue) 
            {
                value = std::nullopt;
            }
        });

        return value;
    }

    void ForEachStyle(std::function<void(StyleInterface&)> operation)
    {
        m_enumerator(operation);
    }

    StyleEnumerator m_enumerator;
};