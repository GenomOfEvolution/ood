#pragma once
#include <iostream>

auto QuackBehavior = []()	
{
	std::cout << "Quack Quack!!!" << std::endl;
};

auto SqueakBehavior = []()	
{
	std::cout << "Squeek!!!" << std::endl;
};

auto MuteQuackBehavior = []()
{

};