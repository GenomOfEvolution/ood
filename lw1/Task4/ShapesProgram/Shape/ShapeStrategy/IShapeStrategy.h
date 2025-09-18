#pragma once
#include "../../gfx/ICanvas.h"

namespace shapes
{
    class IShapeStrategy 
    {
    public:
        virtual ~IShapeStrategy() = default;
        virtual void Draw(gfx::ICanvas* canvas, gfx::Color color) const = 0;
        virtual void Move(double dx, double dy) = 0;
    };
}