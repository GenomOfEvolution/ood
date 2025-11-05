#include "drawer.h"
#include <cstdlib>
#include <cassert>
#include <iostream>

namespace
{
	int Sign(int value)
	{
		return (0 < value) - (value < 0);
	}

	/**
	 * Рисование крутой линии (для случая |to.y - from.y| >= |to.x - from.x|).
	 */
	void DrawSteepLine(Image& image, Point from, Point to, char color)
	{
		const int deltaX = std::abs(to.x - from.x);
		const int deltaY = std::abs(to.y - from.y);

		assert(deltaY >= deltaX);

		if (from.y > to.y)
		{ // Чтобы координата Y возрастала от from к to.
			std::swap(from, to);
		}

		const int stepX = Sign(to.x - from.x); // Шаг по оси X (-1, 0 или 1).
		const int errorThreshold = deltaY + 1; // Порог для изменения координаты X.
		const int deltaErr = deltaX + 1; // Для накопления ошибки.

		// Начальное значение ошибки берем не 0, а deltaErr/2, чтобы избежать дробей.
		int error = deltaErr / 2;

		for (Point p = from; p.y <= to.y; ++p.y)
		{
			image.SetPixel({ p.x, p.y }, color);
			assert((p.y != to.y) || (p.x == to.x));

			error += deltaErr; // Увеличиваем накопленную ошибку.

			if (error >= errorThreshold)
			{ // Если пора изменить координату X
				p.x += stepX; // изменяем на шаг по оси X
				error -= errorThreshold; // сбрасываем ошибку
			}
		}
	}

	/**
	 * Рисование пологой линии (для случая |to.x - from.x| >= |to.y - from.y|).
	 */
	void DrawSlopeLine(Image& image, Point from, Point to, char color)
	{
		const int deltaX = std::abs(to.x - from.x);
		const int deltaY = std::abs(to.y - from.y);

		assert(deltaX >= deltaY);

		if (from.x > to.x)
		{ // Чтобы координата X возрастала от from к to.
			std::swap(from, to);
		}

		// Алгоритм идентичен DrawSteepLine с заменой X на Y.

		const int stepY = Sign(to.y - from.y);
		const int errorThreshold = deltaX + 1;
		const int deltaErr = deltaY + 1;

		int error = deltaErr / 2;

		for (Point p = from; p.x <= to.x; ++p.x)
		{
			image.SetPixel({ p.x, p.y }, color);
			assert((p.x != to.x) || (p.y == to.y));

			error += deltaErr;

			if (error >= errorThreshold)
			{
				p.y += stepY;
				error -= errorThreshold;
			}
		}
	}

	void DrawMirrorPoints(Image& image, Point center, int x, int y, char color)
	{
		std::cout << center.x + x << " " << center.y - y << std::endl;

		int actualY = -y;  

		image.SetPixel(Point(center.x + x, center.y + actualY), color);
		image.SetPixel(Point(center.x + x, center.y - actualY), color);

		image.SetPixel(Point(center.x - x, center.y + actualY), color);
		image.SetPixel(Point(center.x - x, center.y - actualY), color);

		image.SetPixel(Point(center.x + actualY, center.y + x), color);
		image.SetPixel(Point(center.x + actualY, center.y - x), color);

		image.SetPixel(Point(center.x - actualY, center.y + x), color);
		image.SetPixel(Point(center.x - actualY, center.y - x), color);
	}

	void DrawMirrorLines(Image& image, Point center, int x, int y, char color)
	{
		int actualY = -y;

		DrawLine(image, 
			Point(center.x + x, center.y + actualY), 
			Point(center.x + x, center.y - actualY), color);

		DrawLine(image,
			Point(center.x - x, center.y + actualY),
			Point(center.x - x, center.y - actualY), color);

		DrawLine(image,
			Point(center.x + actualY, center.y + x),
			Point(center.x + actualY, center.y - x), color);

		DrawLine(image,
			Point(center.x - actualY, center.y + x),
			Point(center.x - actualY, center.y - x), color);
	}

} // namespace

void DrawLine(Image& image, Point from, Point to, char color)
{
	const int deltaX = std::abs(to.x - from.x);
	const int deltaY = std::abs(to.y - from.y);

	if (deltaY > deltaX)
	{ // Крутая линия.
		DrawSteepLine(image, from, to, color);
	}
	else
	{ // Полая линия.
		DrawSlopeLine(image, from, to, color);
	}
}

void DrawCircle(Image& image, Point center, int radius, char color)
{
	if (radius < 0) 
	{
		return;
	}

	int x = 0;
	int y = -radius;  
	int F_M = 1 - radius;
	int d_e = 4;
	int d_ne = -(radius << 1) + 5; 

	DrawMirrorPoints(image, center, x, y, color);

	while (x < -y) 
	{  
		if (F_M < 0) 
		{
			F_M += d_e;
		}
		else 
		{
			F_M += d_ne;
			d_ne += 2;
			y += 1;
		}
		d_e += 2;
		d_ne += 2;
		x += 1;
		DrawMirrorPoints(image, center, x, y, color);
	}
}

void FillCircle(Image& image, Point center, int radius, char color)
{
	if (radius < 0)
	{
		return;
	}

	int x = 0;
	int y = -radius;
	int F_M = 1 - radius;
	int d_e = 4;
	int d_ne = -(radius << 1) + 5;

	DrawMirrorLines(image, center, x, y, color);

	while (x < -y)
	{
		if (F_M < 0)
		{
			F_M += d_e;
		}
		else
		{
			F_M += d_ne;
			d_ne += 2;
			y += 1;
		}
		d_e += 2;
		d_ne += 2;
		x += 1;
		DrawMirrorLines(image, center, x, y, color);
	}
}