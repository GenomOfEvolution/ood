#pragma once
#include "Geometry.h"
#include <vector>
#include <string>

#ifndef UNIT_TESTS
#include <qmetatype.h>
#endif

struct DocItemPreview
{
	enum class ItemType { Triangle, Rectangle, Ellipse, Image, None };
	ItemType m_type = ItemType::None;
	std::vector<Point> m_points;
	Rect m_boundingBox;
	std::string m_imgPath;
	size_t m_index = 0;
};

#ifndef UNIT_TESTS
Q_DECLARE_METATYPE(DocItemPreview);
#endif