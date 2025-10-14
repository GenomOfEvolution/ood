#include "Beverages/Cappuccino/Cappuccino.h"
#include "Beverages/Coffee/Coffee.h"
#include "Beverages/Latte/Latte.h"
#include "Beverages/Milkshake/Milkshake.h"
#include "Beverages/Tea/Tea.h"

#include "Condiments/ChocolateCrumbs/ChocolateCrumbs.h"
#include "Condiments/ChocolateSlices/ChocolateSlices.h"
#include "Condiments/Cinnamon/Cinnamon.h"
#include "Condiments/CoconutFlakes/CoconutFlakes.h"
#include "Condiments/Cream/Cream.h"
#include "Condiments/IceCubes/IceCubes.h"
#include "Condiments/Lemon/Lemon.h"
#include "Condiments/Liqueur/Liqueur.h"
#include "Condiments/Syrup/Syrup.h"

#include<iostream>

// TODO: добавить диаграмму
int main()
{
	auto coffe = std::make_unique<Coffee>();
	auto coffeWithCrumbs = std::move(std::make_unique<ChocolateCrumbs>(std::move(coffe), 5));
	auto coffeWithCrumbsAndCream = std::move(std::make_unique<Cream>(std::move(coffeWithCrumbs)));

	std::cout << coffeWithCrumbsAndCream->GetCost() << "\n";
	std::cout <<  coffeWithCrumbsAndCream->GetDescription();

	return EXIT_SUCCESS;
}

