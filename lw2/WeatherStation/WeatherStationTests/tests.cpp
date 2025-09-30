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
			observable.RegisterObserver(observer);
			
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
				observable.RegisterObserver(dummyObserver1);
				observable.RegisterObserver(dummyObserver2);

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
