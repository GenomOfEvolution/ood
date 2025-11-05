#include <cmath>
#include <iostream>

#include "Drawer/Drawer.h"

constexpr int STAR_COUNT = 20;

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
		Image img{ { 21, 21 }, '.' };

		/*DrawLine(img, { 3, 2 }, { 26, 5 }, '#');
		DrawLine(img, { 26, 5 }, { 21, 18 }, '#');
		DrawLine(img, { 21, 18 }, { 3, 2 }, '#');*/

		FillCircle(img, { 10, 10 }, 10, '#');

		Print(img, std::cout);
	}

	{
		Image img{ { 10, 10 }, '.' };

		/*DrawLine(img, { 3, 2 }, { 26, 5 }, '#');
		DrawLine(img, { 26, 5 }, { 21, 18 }, '#');
		DrawLine(img, { 21, 18 }, { 3, 2 }, '#');*/

		FillCircle(img, { 4, 4 }, 4, '#');

		Print(img, std::cout);
	}

	return EXIT_SUCCESS;
}