#include "WeatherDataPro/WeatherDataPro.h"
#include "Display/CDisplay/CDisplay.h"
#include "Display/StatsDisplay/StatsDisplay.h"

int main()
{
    CWeatherDataPro wdPro;

    CDisplay display;
    CStatsDisplay statsDisplay;

    wdPro.RegisterObserver(display, 1);
    wdPro.RegisterObserver(statsDisplay, 1);

    wdPro.SetMeasurements(3, 0.7, 760, 3, 90);
    wdPro.SetMeasurements(5, 0.8, 761, 5, 300);

    return EXIT_SUCCESS;
}
