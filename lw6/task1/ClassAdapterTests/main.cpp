#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <sstream>
#include "../ClassAdapter/App/Adapter/ModernToOldLibAdapter.h"

using namespace app;

TEST_CASE("ModernToOldLibAdapter correctly initializes and finalizes drawing")
{
    std::ostringstream out;
    {
        ModernToOldLibAdapter adapter(out);
        REQUIRE(out.str() == "<draw>\n");
    }
    REQUIRE(out.str() == "<draw>\n</draw>\n");
}

TEST_CASE("LineTo draws line from current position")
{
    std::ostringstream out;
    {
        ModernToOldLibAdapter adapter(out);
        out.str("");

        adapter.MoveTo(10, 20);
        adapter.LineTo(30, 40);

        REQUIRE(out.str() == R"(  <line fromX="10" fromY="20" toX="30" toY="40"/>)"
            "\n");
    }
}

TEST_CASE("Multiple LineTo use updated position after MoveTo")
{
    std::ostringstream out;
    {
        ModernToOldLibAdapter adapter(out);
        out.str("");

        adapter.MoveTo(5, 5);
        adapter.LineTo(10, 10);
        adapter.MoveTo(20, 20);
        adapter.LineTo(25, 25);

        REQUIRE(out.str() ==
            R"(  <line fromX="5" fromY="5" toX="10" toY="10"/>)" "\n"
            R"(  <line fromX="20" fromY="20" toX="25" toY="25"/>)" "\n"
        );
    }
}

TEST_CASE("Sequential LineTo without MoveTo reuse same start point")
{
    std::ostringstream out;
    {
        ModernToOldLibAdapter adapter(out);
        out.str("");

        adapter.MoveTo(1, 1);
        adapter.LineTo(2, 2);
        adapter.LineTo(3, 3);

        REQUIRE(out.str() ==
            R"(  <line fromX="1" fromY="1" toX="2" toY="2"/>)" "\n"
            R"(  <line fromX="1" fromY="1" toX="3" toY="3"/>)" "\n"
        );
    }
}

TEST_CASE("Adapter handles negative coordinates")
{
    std::ostringstream out;
    {
        ModernToOldLibAdapter adapter(out);
        out.str("");

        adapter.MoveTo(-10, -20);
        adapter.LineTo(5, -5);

        REQUIRE(out.str() ==
            R"(  <line fromX="-10" fromY="-20" toX="5" toY="-5"/>)" "\n"
        );
    }
}