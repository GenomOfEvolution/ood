#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "Observable/WeatherData.h"
#include "Observable/OutsideWeatherData.h"

#include "TestClasses/BrokenDisplay.h"
#include "TestClasses/CallOrderDisplay.h"
#include "TestClasses/MockDisplay.h"

TEST_CASE("WSProDuo Tests")
{
	GIVEN("Inside and outside weather data")
	{
		WeatherData inside;
		OutsideWeatherData outside;

		GIVEN("Broken display, that registers himself")
		{
			BrokenDisplay display { inside };
			inside.RegisterObserver(display, 0);

			WHEN("it tries to delete himself and calls twice")
			{
				inside.SetMeasurements(1, 1, 1);
				inside.SetMeasurements(1, 1, 1);

				THEN("He deleted successfully")
				{
					REQUIRE(display.GetCallCount() == 1);
				}
			}

			WHEN("removing same display twice")
			{
				inside.RemoveObserver(display);

				THEN("Program should work correctly")
				{
					REQUIRE_NOTHROW(inside.RemoveObserver(display));
				}
			}
		}

		GIVEN("A few call order displays, with different priorities")
		{
			std::vector<int> callOrder;

			CallOrderDisplay display1(callOrder, 1);
			CallOrderDisplay display2(callOrder, 2);
			CallOrderDisplay display3(callOrder, 3);
			CallOrderDisplay display4(callOrder, 4);

			inside.RegisterObserver(display1, 5);
			inside.RegisterObserver(display3, 10);
			inside.RegisterObserver(display2, 10);
			inside.RegisterObserver(display4, 3);

			WHEN("They notified")
			{
				inside.SetMeasurements(1, 1, 1);

				THEN("They are called by their priority")
				{
					REQUIRE(callOrder.size() == 4);

					REQUIRE(callOrder[0] == 3); // 10 приортеты
					REQUIRE(callOrder[1] == 2);
					REQUIRE(callOrder[2] == 1); // 5 приоритет
					REQUIRE(callOrder[3] == 4); // 3 приоритет
				}
			}
		}

		GIVEN("Single order display")
		{
			std::vector<int> callOrder;
			CallOrderDisplay display(callOrder, 1);
			inside.RegisterObserver(display, 10);

			WHEN("observer registered twice")
			{
				inside.RegisterObserver(display, 123);
				inside.SetMeasurements(1, 1, 1);

				THEN("it's only called once")
				{
					REQUIRE(callOrder.size() == 1);
				}
			}
			
		}

		GIVEN("A mock display")
		{
			MockDisplay display(inside, outside);

			WHEN("Inside data changes")
			{
				inside.SetMeasurements(1, 1, 1);
				THEN("display is called once")
				{
					REQUIRE(display.GetCallCount() == 1);
				}

				AND_WHEN("Outside data changes")
				{
					outside.SetMeasurements(1, 1, 1, 10, 30);
					THEN("display is called twice")
					{
						REQUIRE(display.GetCallCount() == 2);
					}
				}
			}
		}
	}
}