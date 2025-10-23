#pragma once
#include "ICanvas.h"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace graphics_lib
{
    class CCanvas : public ICanvas
    {
    public:
        void SetColor(uint32_t rgbColor) override
        {
            std::ostringstream oss;
            oss << std::hex << std::uppercase << std::setfill('0') << std::setw(6) << rgbColor;
            std::cout << "Color (#" << oss.str() << ")" << std::endl;
        }

        void MoveTo(int x, int y) override
        {
            std::cout << "MoveTo (" << x << ", " << y << ")" << std::endl;
        }

        void LineTo(int x, int y) override
        {
            std::cout << "LineTo (" << x << ", " << y << ")" << std::endl;
        }
    };

}//namespace