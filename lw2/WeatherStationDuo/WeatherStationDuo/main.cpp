#include "WeatherData/WeatherData.h"
#include "Display/StatsDisplay/StatsDisplay.h"
#include "Display/CDisplay/CDisplay.h"

int main()
{
    CWeatherData wdIn;
    CWeatherData wdOut;

    // TODO: дисплей сам подписываетс€ на внутр и внеш данные
    CDisplay display;

    wdIn.RegisterObserver(display);
    display.RegisterObservable(wdIn, "in");

    wdOut.RegisterObserver(display);
    display.RegisterObservable(wdOut, "out");

    CStatsDisplay statsDisplay;

    wdIn.RegisterObserver(statsDisplay);
    statsDisplay.RegisterObservable(wdIn, "inside");

    wdOut.RegisterObserver(statsDisplay);
    statsDisplay.RegisterObservable(wdOut, "outside");

    wdIn.SetMeasurements(22, 0.8, 760);
    wdOut.SetMeasurements(15, 0.7, 765);

    wdIn.SetMeasurements(20, 0.75, 761);
    wdOut.SetMeasurements(14, 0.8, 760);

    return EXIT_SUCCESS;
}

