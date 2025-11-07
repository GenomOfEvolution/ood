#include "drawer.h"
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <functional>

namespace
{
	int Sign(int value)
	{
		return (0 < value) - (value < 0);
	}

	/**
	 * Рисование крутой линии (для случая |to.y - from.y| >= |to.x - from.x|).
	 */
	void DrawSteepLine(Image& image, Point from, Point to, uint32_t color)
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
	void DrawSlopeLine(Image& image, Point from, Point to, uint32_t color)
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

	void DrawMirrorPoints(Image& image, Point center, int x, int y, uint32_t color)
	{
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

	void DrawMirrorLines(Image& image, Point center, int x, int y, uint32_t color)
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

	/**
	 * Общая функция для рисования круга с использованием алгоритма Брезенхэма.
	 * Принимает функцию отрисовки, которая определяет, как рисовать точки (контур или заполнение).
	 */
	template<typename DrawFunc>
	void ProcessCircle(Image& image, Point center, int radius, uint32_t color, DrawFunc drawFunc)
	{
		if (radius < 0)
		{
			return;
		}

		// Текущие координаты относительно центра окружности
		int currentX = 0;
		int currentY = -radius;  // Начинаем с самой верхней точки

		// Параметр решения (определяет следующую точку)
		int decisionParameter = 1 - radius;

		// Приращения для параметра решения:
		int deltaEast = 4;        // При движении только по X
		int deltaNorthEast = -(radius << 1) + 5;  // При движении по X и Y

		// Рисуем начальные точки/линии
		drawFunc(image, center, currentX, currentY, color);

		// Основной цикл (проходим 1/8 окружности)
		while (currentX < -currentY)
		{
			if (decisionParameter < 0)
			{
				// Выбираем восточный пиксель (только увеличиваем X)
				decisionParameter += deltaEast;
			}
			else
			{
				// Выбираем северо-восточный пиксель (увеличиваем X и Y)
				decisionParameter += deltaNorthEast;
				deltaNorthEast += 2;  // Корректируем приращение
				currentY += 1;        // Двигаемся по Y
			}

			// Обновляем приращения для следующей итерации
			deltaEast += 2;
			deltaNorthEast += 2;
			currentX += 1;           // Всегда двигаемся по X

			// Рисуем симметричные точки/линии
			drawFunc(image, center, currentX, currentY, color);
		}
	}

} // namespace

void DrawLine(Image& image, Point from, Point to, uint32_t color)
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

void DrawCircle(Image& image, Point center, int radius, uint32_t color)
{
	ProcessCircle(image, center, radius, color, DrawMirrorPoints);
}

void FillCircle(Image& image, Point center, int radius, uint32_t color)
{
	ProcessCircle(image, center, radius, color, DrawMirrorLines);
}