#pragma once
#include <iostream>

auto CantDance = []()
	{
	};

auto MinuetDance = []()
	{
		std::cout << "I'm dancing minuet!" << std::endl;
	};

auto WaltzDance = []()
	{
		std::cout << "I'm dancing waltz!" << std::endl;
	};