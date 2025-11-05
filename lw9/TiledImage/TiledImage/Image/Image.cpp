#include "Image.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <vector>

Image::Image(Size size, char color)
{
	if (size.height < 0 || size.width < 0)
	{
		throw std::out_of_range("Size of image must be not negative");
	}

	m_size = size;

	int widthTiles = (size.width + Tile::SIZE - 1) / Tile::SIZE;
	int heightTiles = (size.height + Tile::SIZE - 1) / Tile::SIZE;

	m_tiles.resize(heightTiles);
	for (auto& row : m_tiles)
	{
		row.resize(widthTiles, CoW<Tile>(Tile(color)));
	}
}

Size Image::GetSize() const noexcept
{
	return m_size;
}

char Image::GetPixel(Point p) const noexcept
{
	if (!IsPointInSize(p, m_size))
	{
		return ' ';
	}

	int tileX = p.x / Tile::SIZE;
	int tileY = p.y / Tile::SIZE;
	Point inTile{ p.x % Tile::SIZE, p.y % Tile::SIZE };

	return m_tiles[tileY][tileX]->GetPixel(inTile);
}

void Image::SetPixel(Point p, char color)
{
	if (!IsPointInSize(p, m_size))
	{
		return;
	}

	int tileX = p.x / Tile::SIZE;
	int tileY = p.y / Tile::SIZE;
	Point inTile{ p.x % Tile::SIZE, p.y % Tile::SIZE };

	m_tiles[tileY][tileX].Write()->SetPixel(inTile, color);
}

void Print(const Image& img, std::ostream& out)
{
	const auto size = img.GetSize();
	for (int y = 0; y < size.height; ++y)
	{
		for (int x = 0; x < size.width; ++x)
		{
			out.put(img.GetPixel({ x, y }));
		}
		out.put('\n');
	}
}

Image LoadImage(const std::string& pixels)
{
	std::istringstream s(pixels);
	Size size;
	std::string line;
	while (std::getline(s, line))
	{
		size.width = std::max(size.width, static_cast<int>(line.length()));
		++size.height;
	}

	Image img(size);

	s = std::istringstream(pixels);
	for (int y = 0; y < size.height; ++y)
	{
		if (!std::getline(s, line))
			break;

		int x = 0;
		for (char ch : line)
		{
			img.SetPixel({ x++, y }, ch);
		}
	}

	return img;
}
