#pragma once
#include <array>
#include <cassert>

#include "../Shared/Geometry.h"

class Tile
{
public:
	// Размер тайла 8*8 пикселей.
	constexpr static int SIZE = 8;

	// Конструктор по умолчанию. Заполняет тайл указанным цветом.
	Tile(char color = ' ') noexcept
	{
		m_pixels.fill(color);
		// -------------- не удалять ------------
		assert(m_instanceCount >= 0);
		++m_instanceCount; // Увеличиваем счётчик тайлов (для целей тестирования).
		// -------------- не удалять ------------
	}

	Tile(const Tile& other)
	{
		m_pixels = other.m_pixels;

		// -------------- не удалять ------------
		assert(m_instanceCount >= 0);
		++m_instanceCount; // Увеличиваем счётчик тайлов (для целей тестирования).
		// -------------- не удалять ------------
	}

	~Tile()
	{
		// -------------- не удалять ------------
		--m_instanceCount; // Уменьшаем счётчик тайлов.
		assert(m_instanceCount >= 0);
		// -------------- не удалять ------------
	}

	/**
	 * Изменяет цвет пикселя тайла.
	 * Если координаты выходят за пределы тайла, метод ничего не делает.
	 */
	void SetPixel(Point p, char color) noexcept
	{
		/* Реализуйте недостающий код самостоятельно. */
		if (IsPointInBound(p))
		{
			m_pixels[size_t(p.x + p.y * SIZE)] = color;
		}
	}

	/**
	 * Возвращает цвет пикселя. Если координаты выходят за пределы тайла, возвращается пробел.
	 */
	char GetPixel(Point p) const noexcept
	{
		if (!IsPointInBound(p))
		{
			return ' ';
		}
		return m_pixels[size_t(p.x + p.y * SIZE)];
	}

	// Возвращает количество экземпляра класса Tile в программе.
	static int GetInstanceCount() noexcept
	{
		// -------------- не удалять ------------
		return m_instanceCount;
		// -------------- не удалять ------------
	}

private:
	bool IsPointInBound(Point p) const
	{
		return (p.x + p.y * SIZE >= 0) && (p.x + p.y * SIZE < SIZE * SIZE);
	}

	// -------------- не удалять ------------
	inline static int m_instanceCount = 0;
	// -------------- не удалять ------------

	std::array<char, SIZE* SIZE> m_pixels;
};