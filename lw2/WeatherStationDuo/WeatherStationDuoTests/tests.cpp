#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../WeatherStationDuo/WeatherData/WeatherData.h"
#include "../WeatherStationDuo/Observer/IObserver.h"
#include <sstream>
#include <map>

TEST_CASE("Several observables")
{
	GIVEN("Two observables")
	{
		CWeatherData obs1;
		CWeatherData obs2;

		AND_GIVEN("dummy observer")
		{
			class DummyObserver : public IObserver<SWeatherInfo>
			{
			public:
				DummyObserver(std::ostream& out) 
					: m_out(out)
				{ }

				void SetObservableName(IObservable<SWeatherInfo>& observable, std::string const& name)
				{
					m_observables[&observable] = name;
				}

				void RemoveObservable(IObservable<SWeatherInfo>& observable)
				{
					m_observables.erase(&observable);
				}

			private:
				void Update(IObservable<SWeatherInfo>& observable, SWeatherInfo const& data) override
				{
					auto name = m_observables.find(&observable);
					if (name != m_observables.end())
					{
						m_out << name->second;
					}
					else
					{
						m_out << "unknown";
					}

					m_out << '\n';
				}

				std::ostream& m_out;
				std::map<IObservable<SWeatherInfo>*, std::string> m_observables;
			};

			std::ostringstream out;
			DummyObserver observer(out);

			obs1.RegisterObserver(observer);
			obs2.RegisterObserver(observer);

			WHEN("notifying from both unitilized observers")
			{
				obs1.NotifyObservers();
				obs2.NotifyObservers();

				THEN("both of them are unknown")
				{
					REQUIRE(out.str() == "unknown\nunknown\n");
				}
			}

			WHEN("observables have setted their names")
			{
				observer.SetObservableName(obs1, "1");
				observer.SetObservableName(obs2, "2");

				AND_WHEN("notifying from both observables")
				{
					obs1.NotifyObservers();
					obs2.NotifyObservers();

					THEN("they have different names")
					{
						REQUIRE(out.str() == "1\n2\n");
					}
				}

				AND_WHEN("removing info about first observable")
				{
					observer.RemoveObservable(obs1);

					AND_WHEN("notifying from both observables")
					{
						obs1.NotifyObservers();
						obs2.NotifyObservers();

						THEN("first one is unknown")
						{
							REQUIRE(out.str() == "unknown\n2\n");
						}
					}
				}
			}
		}
	}
}
