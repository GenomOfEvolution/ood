#include "CDisplay.h"
#include <iostream>

void CDisplay::Update(SWeatherInfo const& data)
{
	std::cout << "Current Temp " << data.temperature << std::endl;
	std::cout << "Current Hum " << data.humidity << std::endl;
	std::cout << "Current Pressure " << data.pressure << std::endl;
	std::cout << "----------------" << std::endl;
}