#pragma once
#include "../Image/Image.h"

/*
 * Рисует отрезок прямой линии между точками from и to цветом color на изображении Image.
 */
void DrawLine(Image& image, Point from, Point to, char color);

/*
 * Рисует окружность с центром в точке center и радиусом radius цветом color на изображении Image.
 */
void DrawCircle(Image& image, Point center, int radius, char color);

void FillCircle(Image& image, Point center, int radius, char color);