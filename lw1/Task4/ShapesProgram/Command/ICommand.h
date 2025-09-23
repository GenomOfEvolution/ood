#pragma once
#include <iostream>

namespace UI
{
    class ICommand 
    {
    public:
        virtual ~ICommand() = default;
        virtual void Execute(std::istream& input) = 0;
    };
}; // namespace UI