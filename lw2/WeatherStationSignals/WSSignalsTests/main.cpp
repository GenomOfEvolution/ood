#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "Observable/WeatherData.h"
#include <vector>

using boost::signals2::connection;

TEST_CASE("")
{
	GIVEN("weather data")
	{
		WeatherData weatherData;

		GIVEN("call count and connections")
		{
			int callCount = 0;
			connection conn1 = weatherData.DoOnTemperatureChanged([&callCount](const WeatherInfo&) { ++callCount; });
			connection conn2 = weatherData.DoOnPressureChanged([&callCount](const WeatherInfo&) { ++callCount; });

			WHEN("temperature changed")
			{
				weatherData.SetTemperature(25.0);
				THEN("call count increases")
				{
					REQUIRE(callCount == 1);
				}

				WHEN("Pressure changed")
				{
					weatherData.SetPressure(1);
					THEN("call count increases")
					{
						REQUIRE(callCount == 2);
					}

					WHEN("humidity changes")
					{
						weatherData.SetHumidity(1);
						THEN("call count NOT increased")
						{
							REQUIRE(callCount == 2);
						}
					}
				}
			}
		}

		GIVEN("a few connections")
		{
			std::vector<int> callOrder;
			connection conn1 = weatherData.DoOnTemperatureChanged(
				[&callOrder](const WeatherInfo&) { callOrder.push_back(1); },
				5);
			connection conn2 = weatherData.DoOnTemperatureChanged(
				[&callOrder](const WeatherInfo&) { callOrder.push_back(2); },
				10);
			connection conn3 = weatherData.DoOnTemperatureChanged(
				[&callOrder](const WeatherInfo&) { callOrder.push_back(3); },
				3);

			WHEN("temperature changes")
			{
				weatherData.SetTemperature(25.0);

				THEN("connections called in priority")
				{
					REQUIRE(callOrder.size() == 3);
					REQUIRE(callOrder[0] == 2); // Приоритет 10
					REQUIRE(callOrder[1] == 1); // Приоритет 5
					REQUIRE(callOrder[2] == 3); // Приоритет 3
				}
			}
		}

		GIVEN("a few connections with their call counters")
		{
			int subscriber1CallCount = 0;
			int subscriber2CallCount = 0;
			int subscriber3CallCount = 0;
			connection conn1 = weatherData.DoOnTemperatureChanged([&subscriber1CallCount](const WeatherInfo&) { ++subscriber1CallCount; });
			connection conn2 = weatherData.DoOnTemperatureChanged([&subscriber2CallCount](const WeatherInfo&) { ++subscriber2CallCount; });
			connection conn3 = weatherData.DoOnTemperatureChanged([&subscriber3CallCount](const WeatherInfo&) { ++subscriber3CallCount; });

			WHEN("temperature changed")
			{
				weatherData.SetTemperature(25.0);

				THEN("all of them called once")
				{
					REQUIRE(subscriber1CallCount == 1);
					REQUIRE(subscriber2CallCount == 1);
					REQUIRE(subscriber3CallCount == 1);
				}
			}
		}

		GIVEN("connections with all event triggers")
		{
			int temperatureCallCount = 0;
			int humidityCallCount = 0;
			int pressureCallCount = 0;
			int windSpeedCallCount = 0;
			int windDirectionCallCount = 0;

			auto conn1 = weatherData.DoOnTemperatureChanged([&temperatureCallCount](const WeatherInfo&) { ++temperatureCallCount; });
			auto conn2 = weatherData.DoOnHumidityChanged([&humidityCallCount](const WeatherInfo&) { ++humidityCallCount; });
			auto conn3 = weatherData.DoOnPressureChanged([&pressureCallCount](const WeatherInfo&) { ++pressureCallCount; });
			auto conn4 = weatherData.DoOnWindSpeedChanged([&windSpeedCallCount](const WeatherInfo&) { ++windSpeedCallCount; });
			auto conn5 = weatherData.DoOnWindDirectionChanged([&windDirectionCallCount](const WeatherInfo&) { ++windDirectionCallCount; });

			WHEN("all weather changed")
			{
				weatherData.SetMeasurements(25.0, 60.0, 760.0, 15.0, 180.0);

				THEN("all of them called once")
				{
					REQUIRE(temperatureCallCount == 1);
					REQUIRE(humidityCallCount == 1);
					REQUIRE(pressureCallCount == 1);
					REQUIRE(windSpeedCallCount == 1);
					REQUIRE(windDirectionCallCount == 1);
				}
			}
		}

		GIVEN("weather info data, with all connections")
		{
			WeatherInfo receivedData;
			auto conn1 = weatherData.DoOnTemperatureChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });
			auto conn2 = weatherData.DoOnHumidityChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });
			auto conn3 = weatherData.DoOnPressureChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });
			auto conn4 = weatherData.DoOnWindSpeedChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });
			auto conn5 = weatherData.DoOnWindDirectionChanged([&receivedData](const WeatherInfo& data) { receivedData = data; });

			WHEN("mesaurment changed")
			{
				weatherData.SetMeasurements(25.5, 65.3, 755.2, 12.7, 270.5);

				THEN("data setted correctly")
				{
					REQUIRE(receivedData.temperature == 25.5);
					REQUIRE(receivedData.humidity == 65.3);
					REQUIRE(receivedData.pressure == 755.2);
					REQUIRE(receivedData.windSpeed == 12.7);
					REQUIRE(receivedData.windDirection == 270.5);
				}
			}
		}

		GIVEN("2 connections")
		{
			int subscriber1CallCount = 0;
			int subscriber2CallCount = 0;
			connection conn1 = weatherData.DoOnTemperatureChanged([&subscriber1CallCount](const WeatherInfo&) { ++subscriber1CallCount; });
			connection conn2 = weatherData.DoOnTemperatureChanged([&subscriber2CallCount](const WeatherInfo&) { ++subscriber2CallCount; });

			WHEN("temperature changed")
			{
				weatherData.SetTemperature(25.0);

				THEN("connection called once")
				{
					REQUIRE(subscriber1CallCount == 1);
					REQUIRE(subscriber2CallCount == 1);
				}

				WHEN("first connection disconected")
				{
					conn1.disconnect();
					weatherData.SetTemperature(30.0);
					THEN("only second connection activated")
					{
						REQUIRE(subscriber1CallCount == 1);
						REQUIRE(subscriber2CallCount == 2);
					}
				}
			}
		}
	}
}