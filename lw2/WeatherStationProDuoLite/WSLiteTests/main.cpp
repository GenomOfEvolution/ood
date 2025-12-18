#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "Observable/WeatherData.h"
#include "CountingObserver.h"
#include "OrderTrackingObserver.h"

TEST_CASE("")
{
	GIVEN("weather data")
	{
		WeatherData weatherData;

		GIVEN("counting observer")
		{
			CountingObserver observer;
			weatherData.RegisterObserver(observer, 1, WeatherType::Temperature);
		
			WHEN("weather data changes tempreature")
			{
				weatherData.SetTemperature(10);

				THEN("observer called once, via temperature event")
				{
					REQUIRE(observer.GetCallCount() == 1);
					REQUIRE(observer.GetLastEvent() == WeatherType::Temperature);
				}

				WHEN("humidity changes")
				{
					weatherData.SetHumidity(11);
					THEN("observer doesn't changes")
					{
						REQUIRE(observer.GetCallCount() == 1);
					}
				}
			}
		}

		GIVEN("counting observer")
		{
			CountingObserver observer;
			weatherData.RegisterObserver(observer, 1, WeatherType::Temperature);
			weatherData.RegisterObserver(observer, 1, WeatherType::Pressure);

			WHEN("weather data changes tempreature")
			{
				weatherData.SetTemperature(10);

				THEN("observer called once, via temperature event")
				{
					REQUIRE(observer.GetCallCount() == 1);
					REQUIRE(observer.GetLastEvent() == WeatherType::Temperature);
				}

				WHEN("pressure changes")
				{
					weatherData.SetPressure(11);
					THEN("observer called once more, via pressure event")
					{
						REQUIRE(observer.GetCallCount() == 2);
						REQUIRE(observer.GetLastEvent() == WeatherType::Pressure);
					}

					WHEN("humidity changes")
					{
						weatherData.SetHumidity(13);

						THEN("observer doesn't react")
						{
							REQUIRE(observer.GetCallCount() == 2);
						}
					}
				}
			}
		}

		GIVEN("multiple oreder tracking observers")
		{
			std::vector<int> callOrder;
			OrderTrackingObserver obs1(callOrder, 1);
			OrderTrackingObserver obs2(callOrder, 2);
			OrderTrackingObserver obs3(callOrder, 3);

			weatherData.RegisterObserver(obs1, 5, WeatherType::Temperature);
			weatherData.RegisterObserver(obs2, 10, WeatherType::Temperature);
			weatherData.RegisterObserver(obs3, 3, WeatherType::Temperature);

			WHEN("temperature changes")
			{
				weatherData.SetTemperature(24);

				THEN("Multiple observers called with priority")
				{
					REQUIRE(callOrder.size() == 3);
					REQUIRE(callOrder[0] == 2); // Приоритет 10
					REQUIRE(callOrder[1] == 1); // Приоритет 5
					REQUIRE(callOrder[2] == 3); // Приоритет 3
				}
			}
		}

		GIVEN("counting observer")
		{
			CountingObserver observer;
			weatherData.RegisterObserver(observer, 1, WeatherType::Temperature);
		
			WHEN("Temperature changes")
			{
				weatherData.SetTemperature(25.0);

				THEN("observer called once")
				{
					REQUIRE(observer.GetCallCount() == 1);
				}

				WHEN("observer unsubscribed")
				{
					weatherData.RemoveObserver(observer, WeatherType::Temperature);
					weatherData.SetTemperature(30.0);

					THEN("No more inocations from observers")
					{
						REQUIRE(observer.GetCallCount() == 1);
					}
				}
			}
		}

		GIVEN("counting observer")
		{
			CountingObserver observer;

			WHEN("Registrating same oberver multiple times")
			{
				weatherData.RegisterObserver(observer, 1, WeatherType::Temperature);
				weatherData.RegisterObserver(observer, 5, WeatherType::Temperature);

				weatherData.SetTemperature(25.0);

				THEN("Observer called only once")
				{
					REQUIRE(observer.GetCallCount() == 1);
				}
			}

			WHEN("Removing unregistered observer")
			{
				THEN("Program does not throw")
				{
					REQUIRE_NOTHROW(weatherData.RemoveObserver(observer, WeatherType::Temperature));
				}
			}
		}
	}
}