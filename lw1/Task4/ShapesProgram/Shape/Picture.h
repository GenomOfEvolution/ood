#pragma once
#include "Shape.h"
#include "../gfx/ICanvas.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

namespace shapes 
{
    class Picture
    {
    public:
        Picture() = default;

        Shape* GetShape(const std::string& id);
        const Shape* GetShape(const std::string& id) const;

        void AddShape(const std::string& id, std::unique_ptr<Shape> shape);
        void DeleteShape(const std::string& id);
        void MovePicture(double dx, double dy);
        void DrawPicture(gfx::ICanvas* canvas) const;
        std::string GetAllInfo() const;

    private:
        std::unordered_map<std::string, std::unique_ptr<Shape>> m_shapes;
        std::vector<std::string> m_order;
    };
} // namespace shapes