#pragma once
#include "IImage.h"
#include "../Shape/IShape.h" 
#include "../Shape/Rectangle/CRectangle.h"

class CImage : public IImage, public IShape
{
public:
    CImage(const std::string& path, const Point& position, double width, double height);

    std::string GetPath() const override;
    void SetPath(const std::string& path) override;

    bool ContainsPoint(const Point& point) const override;
    Rect GetBoundingBox() const override;
    void MoveBy(const Point& delta) override;
    void Resize(const Rect& newBoundingBox) override;

private:
    std::string m_path;
    CRectangle m_boundingBox;
};