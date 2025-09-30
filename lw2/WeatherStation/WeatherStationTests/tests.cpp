#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../WeatherStation/WeatherData/WeatherData.h"
#include "../WeatherStation/Observer/IObserver.h"

#include <sstream>

TEST_CASE("Safely delete observer")
{
	GIVEN("an observable")
	{
		CWeatherData observable;

		AND_GIVEN("observer, that wants to delete himself")
		{
			class SelfDestructObserver : public IObserver<SWeatherInfo>
			{
			public:
				SelfDestructObserver(std::ostream& out, IObservable<SWeatherInfo>& observable)
					: m_out(out)
					, m_observable(observable)
				{	}

			private:
				std::ostream& m_out;
				IObservable<SWeatherInfo>& m_observable;

				void Update(SWeatherInfo const&) override
				{
					m_observable.RemoveObserver(*this);
					m_out << "I wasn`t deleted!";
				}
			};

			std::ostringstream os;
			SelfDestructObserver observer(os, observable);
			observable.RegisterObserver(observer, 1);
			
			AND_GIVEN("a few dummy observers")
			{
				class CDummyObserver : public IObserver<SWeatherInfo>
				{
				private:
					void Update(SWeatherInfo const&) override
					{	}
				};

				CDummyObserver dummyObserver1;
				CDummyObserver dummyObserver2;
				observable.RegisterObserver(dummyObserver1, 2);
				observable.RegisterObserver(dummyObserver2, 3);

				WHEN("notifying observers")
				{
					observable.NotifyObservers();

					THEN("it doesn't crash and produces correct output")
					{
						REQUIRE(os.str() == "I wasn`t deleted!");
					}
				}
			}
		}
	}
}


TEST_CASE("observers with priority")
{
	GIVEN("an observable")
	{
		CWeatherData observable;

		AND_GIVEN("observers #1 and #2")
		{
			class CObserver : public IObserver<SWeatherInfo>
			{
			public:
				CObserver(std::ostream& out, int number)
					: m_out(out)
					, m_number(number)
				{
				}

			private:
				void Update(SWeatherInfo const&) override
				{
					m_out << "Observer #" << m_number << "\n";
				}

				std::ostream& m_out;
				int m_number;
			};

			std::ostringstream out;
			CObserver observer1(out, 1);
			CObserver observer2(out, 2);

			WHEN("adding #1 with priority 1 and #2 with priority 2 and notifying them")
			{
				observable.RegisterObserver(observer1, 1);
				observable.RegisterObserver(observer2, 2);
				observable.NotifyObservers();

				THEN("#1 gets called first, then #2")
				{
					REQUIRE(out.str() == "Observer #1\nObserver #2\n");
				}
			}

			WHEN("adding #1 with priority 5 and #2 with priority 1 and notifying them")
			{
				observable.RegisterObserver(observer1, 5);
				observable.RegisterObserver(observer2, 1);
				observable.NotifyObservers();

				THEN("#2 gets called first, then #1")
				{
					REQUIRE(out.str() == "Observer #2\nObserver #1\n");
				}
			}

			WHEN("adding both observers with the same priority (#1 first) and notifying them")
			{
				observable.RegisterObserver(observer1, 1);
				observable.RegisterObserver(observer2, 1);
				observable.NotifyObservers();

				THEN("they are called in the order of registering: #1 gets called first, then #2")
				{
					REQUIRE(out.str() == "Observer #1\nObserver #2\n");
				}
			}

			WHEN("adding both observers with the same priority (#2 first) and notifying them")
			{
				observable.RegisterObserver(observer2, 1);
				observable.RegisterObserver(observer1, 1);
				observable.NotifyObservers();

				THEN("they are called in the order of registering: #2 gets called first, then #1")
				{
					REQUIRE(out.str() == "Observer #2\nObserver #1\n");
				}
			}

			WHEN("adding single observer many times")
			{
				observable.RegisterObserver(observer1, 1);

				THEN("observable throws error: already registred an observer")
				{
					REQUIRE_THROWS_AS(observable.RegisterObserver(observer1, 2), std::runtime_error);
				}
			}
		}
	}
}