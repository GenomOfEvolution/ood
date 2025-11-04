#pragma once
#include <iostream>

class ICommand
{
public:
	virtual void Execute(std::istream& input) = 0;
	virtual ~ICommand() = default;
};