#include <iostream>
#include "Display/Display.h"
#include "Display/StatsDisplay.h"
#include "Observable/OutsideWeatherData.h"

int main()
{
	WeatherData indoorSensor;
	OutsideWeatherData outdoorSensor;

	Display display(std::cout, 5, indoorSensor, outdoorSensor);
	StatsDisplay statsDisplay(std::cout, 5, indoorSensor, outdoorSensor);

	indoorSensor.SetMeasurements(3, 0.7, 760);
	std::cout << std::endl;

	outdoorSensor.SetMeasurements(4, 0.8, 761, 15, 45);
	std::cout << std::endl;

	indoorSensor.RegisterObserver(display, 5);
	indoorSensor.RemoveObserver(statsDisplay);

	indoorSensor.SetMeasurements(10, 0.8, 761);
	std::cout << std::endl;
	outdoorSensor.SetMeasurements(-10, 0.8, 761, 15, 45);

    return EXIT_SUCCESS;
}

