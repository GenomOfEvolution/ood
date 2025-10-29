#pragma once
#include "../Common/CommonTypes.h"
#include <vector>
#include <optional>

class ICanvas
{
public:
	virtual void SetLineColor(std::optional<RGBAColor> color) = 0;
	virtual void SetFillColor(std::optional<RGBAColor> color) = 0;
	virtual void SetLineThickness(std::optional<double> thickness) = 0;

	virtual void DrawPolygon(std::vector<PointD> points) = 0;
	virtual void DrawEllipse(const PointD& center, double horizontalRadius, double verticalRadius) = 0;

	virtual ~ICanvas() = default;
};