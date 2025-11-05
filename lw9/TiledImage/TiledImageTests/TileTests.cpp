#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "../TiledImage/Tile/Tile.h"

TEST_CASE("Tile Construction and Destruction") {
    REQUIRE(Tile::GetInstanceCount() == 0);

    SECTION("Default constructor initializes with spaces")
    {
        Tile t;
        REQUIRE(Tile::GetInstanceCount() == 1);

        for (int y = 0; y < Tile::SIZE; ++y) 
        {
            for (int x = 0; x < Tile::SIZE; ++x) 
            {
                REQUIRE(t.GetPixel(Point{ x, y }) == ' ');
            }
        }
    }

    SECTION("Constructor with color fills all pixels") 
    {
        Tile t(0xFFFFFFFF);
        REQUIRE(Tile::GetInstanceCount() == 1);

        for (int y = 0; y < Tile::SIZE; ++y) 
        {
            for (int x = 0; x < Tile::SIZE; ++x) 
            {
                REQUIRE(t.GetPixel(Point{ x, y }) == 0xFFFFFFFF);
            }
        }
    }

    SECTION("Copy constructor creates independent copy") 
    {
        Tile original(0x12345678);
        original.SetPixel(Point{ 1, 1 }, 0x87654321);
        original.SetPixel(Point{ 4, 2 }, 0x55555555);

        SECTION("Copy has same pixel data") 
        {
            Tile copy(original);
            REQUIRE(Tile::GetInstanceCount() == 2);

            for (int y = 0; y < Tile::SIZE; ++y) {
                for (int x = 0; x < Tile::SIZE; ++x) 
                {
                    REQUIRE(original.GetPixel(Point{ x, y }) == copy.GetPixel(Point{ x, y }));
                }
            }
        }

        SECTION("Modifying copy doesn't affect original") 
        {
            Tile copy(original);
            copy.SetPixel(Point{ 1, 1 }, 0x00000000);
            copy.SetPixel(Point{ 5, 5 }, 0x99999999);

            REQUIRE(original.GetPixel(Point{ 1, 1 }) == 0x87654321);
            REQUIRE(copy.GetPixel(Point{ 1, 1 }) == 0x00000000);
            REQUIRE(copy.GetPixel(Point{ 5, 5 }) == 0x99999999);
            REQUIRE(original.GetPixel(Point{ 5, 5 }) == 0x12345678);
        }
    }
}

TEST_CASE("Tile Pixel Operations") 
{
    Tile t;

    SECTION("Setting and getting valid pixels") 
    {
        SECTION("Corners of the tile") 
        {
            t.SetPixel(Point{ 0, 0 }, 0x11111111);
            t.SetPixel(Point{ 7, 0 }, 0x22222222);
            t.SetPixel(Point{ 0, 7 }, 0x33333333);
            t.SetPixel(Point{ 7, 7 }, 0x44444444);

            REQUIRE(t.GetPixel(Point{ 0, 0 }) == 0x11111111);
            REQUIRE(t.GetPixel(Point{ 7, 0 }) == 0x22222222);
            REQUIRE(t.GetPixel(Point{ 0, 7 }) == 0x33333333);
            REQUIRE(t.GetPixel(Point{ 7, 7 }) == 0x44444444);
        }

        SECTION("Middle of the tile") 
        {
            t.SetPixel(Point{ 3, 4 }, 0x55555555);
            t.SetPixel(Point{ 4, 3 }, 0x66666666);

            REQUIRE(t.GetPixel(Point{ 3, 4 }) == 0x55555555);
            REQUIRE(t.GetPixel(Point{ 4, 3 }) == 0x66666666);
        }

        SECTION("Overwriting pixel values") 
        {
            t.SetPixel(Point{ 2, 2 }, 0x77777777);
            REQUIRE(t.GetPixel(Point{ 2, 2 }) == 0x77777777);

            t.SetPixel(Point{ 2, 2 }, 0x88888888);
            REQUIRE(t.GetPixel(Point{ 2, 2 }) == 0x88888888);
        }
    }

    SECTION("Handling invalid coordinates") 
    {
        SECTION("Negative coordinates") 
        {
            t.SetPixel(Point{ -1, 0 }, 0xFFFF0000);
            t.SetPixel(Point{ 0, -1 }, 0xFFFF0000);
            t.SetPixel(Point{ -1, -1 }, 0xFFFF0000);

            REQUIRE(t.GetPixel(Point{ -1, 0 }) == ' ');
            REQUIRE(t.GetPixel(Point{ 0, -1 }) == ' ');
            REQUIRE(t.GetPixel(Point{ -1, -1 }) == ' ');
        }

        SECTION("Invalid coordinates don't affect valid pixels") 
        {
            t.SetPixel(Point{ 1, 1 }, 0xAAAAAAAA);
            t.SetPixel(Point{ 2, 2 }, 0xBBBBBBBB);

            t.SetPixel(Point{ -1, 0 }, 0xFFFF0000);
            t.SetPixel(Point{ 8, 8 }, 0xFFFF0000);

            REQUIRE(t.GetPixel(Point{ 1, 1 }) == 0xAAAAAAAA);
            REQUIRE(t.GetPixel(Point{ 2, 2 }) == 0xBBBBBBBB);

            for (int y = 0; y < Tile::SIZE; ++y) 
            {
                for (int x = 0; x < Tile::SIZE; ++x)
                {
                    if (!(x == 1 && y == 1) && !(x == 2 && y == 2)) 
                    {
                        REQUIRE(t.GetPixel(Point{ x, y }) == ' ');
                    }
                }
            }
        }
    }
}
