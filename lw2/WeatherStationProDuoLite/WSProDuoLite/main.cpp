#include <iostream>
#include "Display/Display.h"
#include "Display/Animals.h"

static void TestWeather()
{
	std::cout << "\n==========Testing Weather==========\n";
    WeatherData sensor;

	Display display1(std::cout);
	Display display2(std::cout);

	sensor.RegisterObserver(display1, 1, WeatherType::WindDirection);
	sensor.RegisterObserver(display1, 1, WeatherType::WindSpeed);
	sensor.RegisterObserver(display1, 1, WeatherType::Temperature);

	sensor.RegisterObserver(display2, 2, WeatherType::Temperature);
	sensor.RegisterObserver(display2, 2, WeatherType::Pressure);
	sensor.RegisterObserver(display2, 2, WeatherType::Humidity);

	std::cout << "\nUpd temperature: \n";
	sensor.SetTemperature(12);

	std::cout << "\nUnsubscribe first observer from tempreature: \n";
	sensor.RemoveObserver(display1, WeatherType::Temperature);
	sensor.SetTemperature(-3);

	std::cout << "\nUpd wind: \n";
	sensor.SetWindDirection(123);
	sensor.SetWindSpeed(20);

	std::cout << "\nUpd Humidity: \n";
	sensor.SetHumidity(30);
}

static void TestAnimals()
{
	std::cout << "\n==========Testing Animals==========\n";
	Cat cat;

	Mouse mouse1("Remi", std::cout);
	Mouse mouse2("Jerry", std::cout);
	Dog dog("Barbos", std::cout);

	cat.RegisterObserver(mouse1, 1, CatEvent::SaidMeow);
	cat.RegisterObserver(mouse1, 1, CatEvent::SleepStateChanged);

	cat.RegisterObserver(mouse2, 2, CatEvent::SaidMeow);
	cat.RegisterObserver(mouse2, 2, CatEvent::SleepStateChanged);

	cat.RegisterObserver(dog, 3, CatEvent::SaidMeow);
	cat.RegisterObserver(dog, 3, CatEvent::LocationChanged);

	std::cout << "\nCat moves to living room: \n";
	cat.MoveTo("living room");

	std::cout << "\nCat says meow: \n";
	cat.SayMeow("Meow-Meow!");

	std::cout << "\nCat goes to sleep: \n";
	cat.GoSleep();

	std::cout << "\nCat wakes up: \n";
	cat.WakeUp();
}

int main()
{
	TestWeather();
	TestAnimals();

    return EXIT_SUCCESS;
}

