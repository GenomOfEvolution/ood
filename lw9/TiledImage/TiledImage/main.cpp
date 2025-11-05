#include <cmath>
#include <iostream>

#include "Drawer/Drawer.h"

int main()
{
	{
		Image img = LoadImage(
			" CCCC             \n"
			"CC  CC   ##    ## \n"
			"CC      ####  ####\n"
			"CC  CC   ##    ## \n"
			" CCCC             \n");
		Print(img, std::cout);
	}

	{
		Image img{ { 40, 40 }, '.' };

		FillCircle(img, { 4, 4 }, 5, '#');
		DrawCircle(img, { 20, 16 }, 4, '#');

		DrawLine(img, { 20, 20 }, { 20, 30 }, '#');
		DrawLine(img, { 20, 20 }, { 15, 25 }, '#');
		DrawLine(img, { 20, 20 }, { 25, 25 }, '#');
		DrawLine(img, { 20, 30 }, { 25, 35 }, '#');
		DrawLine(img, { 20, 30 }, { 15, 35 }, '#');

		Print(img, std::cout);
	}

	{
		Image img{ { 40, 40 }, 0x00ff00 };

		FillCircle(img, { 4, 4 }, 5, 0xffdd00);
		DrawCircle(img, { 20, 16 }, 4, 0x000000);

		DrawLine(img, { 20, 20 }, { 20, 30 }, 0x000000);
		DrawLine(img, { 20, 20 }, { 15, 25 }, 0x000000);
		DrawLine(img, { 20, 20 }, { 25, 25 }, 0x000000);
		DrawLine(img, { 20, 30 }, { 25, 35 }, 0x000000);
		DrawLine(img, { 20, 30 }, { 15, 35 }, 0x000000);

		SaveImageAsPPM(img, "pic.ppm");
	}

	return EXIT_SUCCESS;
}