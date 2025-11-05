#include <catch.hpp>
#include "../TiledImage/Image/Image.h"
#include "../TiledImage/Tile/Tile.h"

TEST_CASE("Image Construction") {
    SECTION("Valid size construction") {
        Image img(Size{ 16, 16 }, 0x12345678);
        REQUIRE(img.GetSize().width == 16);
        REQUIRE(img.GetSize().height == 16);

        for (int y = 0; y < 16; ++y) 
        {
            for (int x = 0; x < 16; ++x) 
            {
                REQUIRE(img.GetPixel(Point{ x, y }) == 0x12345678);
            }
        }
    }

    SECTION("Construction with default color (space)") 
    {
        Image img(Size{ 8, 8 });
        REQUIRE(img.GetSize().width == 8);
        REQUIRE(img.GetSize().height == 8);

        for (int y = 0; y < 8; ++y) 
        {
            for (int x = 0; x < 8; ++x) 
            {
                REQUIRE(img.GetPixel(Point{ x, y }) == ' ');
            }
        }
    }

    SECTION("Invalid size throws exception") 
    {
        REQUIRE_THROWS_AS(Image(Size{ -1, 10 }), std::out_of_range);
        REQUIRE_THROWS_AS(Image(Size{ 10, -1 }), std::out_of_range);
        REQUIRE_THROWS_AS(Image(Size{ -1, -1 }), std::out_of_range);
    }

    SECTION("Image size that requires multiple tiles") 
    {
        SECTION("Exactly one tile") {
            Image img(Size{ 8, 8 }, 0x11111111);
            REQUIRE(img.GetSize().width == 8);
            REQUIRE(img.GetSize().height == 8);
        }

        SECTION("Multiple tiles in both dimensions") 
        {
            Image img(Size{ 20, 20 }, 0x22222222);
            REQUIRE(img.GetSize().width == 20);
            REQUIRE(img.GetSize().height == 20);

            REQUIRE(img.GetPixel(Point{ 0, 0 }) == 0x22222222);
            REQUIRE(img.GetPixel(Point{ 7, 7 }) == 0x22222222);
            REQUIRE(img.GetPixel(Point{ 8, 8 }) == 0x22222222);
            REQUIRE(img.GetPixel(Point{ 19, 19 }) == 0x22222222);
        }

        SECTION("Non-multiple of tile size") 
        {
            Image img(Size{ 10, 15 }, 0x33333333);
            REQUIRE(img.GetSize().width == 10);
            REQUIRE(img.GetSize().height == 15);

            REQUIRE(img.GetPixel(Point{ 9, 14 }) == 0x33333333);
        }
    }
}

TEST_CASE("Image Pixel Operations") 
{
    Image img(Size{ 16, 16 }, ' ');

    SECTION("Setting and getting valid pixels") 
    {
        SECTION("Pixels in different tiles") 
        {
            img.SetPixel(Point{ 0, 0 }, 0x11111111);    // Tile (0,0)
            img.SetPixel(Point{ 7, 7 }, 0x22222222);    // Tile (0,0)
            img.SetPixel(Point{ 8, 0 }, 0x33333333);    // Tile (1,0)
            img.SetPixel(Point{ 0, 8 }, 0x44444444);    // Tile (0,1)
            img.SetPixel(Point{ 15, 15 }, 0x55555555);  // Tile (1,1)

            REQUIRE(img.GetPixel(Point{ 0, 0 }) == 0x11111111);
            REQUIRE(img.GetPixel(Point{ 7, 7 }) == 0x22222222);
            REQUIRE(img.GetPixel(Point{ 8, 0 }) == 0x33333333);
            REQUIRE(img.GetPixel(Point{ 0, 8 }) == 0x44444444);
            REQUIRE(img.GetPixel(Point{ 15, 15 }) == 0x55555555);
        }

        SECTION("Overwriting pixels") 
        {
            img.SetPixel(Point{ 5, 5 }, 0x66666666);
            REQUIRE(img.GetPixel(Point{ 5, 5 }) == 0x66666666);

            img.SetPixel(Point{ 5, 5 }, 0x77777777);
            REQUIRE(img.GetPixel(Point{ 5, 5 }) == 0x77777777);
        }
    }

    SECTION("Handling out-of-bounds pixels") 
    {
        SECTION("GetPixel with out-of-bounds coordinates") 
        {
            REQUIRE(img.GetPixel(Point{ -1, 0 }) == ' ');
            REQUIRE(img.GetPixel(Point{ 0, -1 }) == ' ');
            REQUIRE(img.GetPixel(Point{ 16, 0 }) == ' ');
            REQUIRE(img.GetPixel(Point{ 0, 16 }) == ' ');
            REQUIRE(img.GetPixel(Point{ 100, 100 }) == ' ');
        }

        SECTION("SetPixel with out-of-bounds coordinates is ignored") 
        {
            auto originalColor = img.GetPixel(Point{ 0, 0 });

            img.SetPixel(Point{ -1, 0 }, 0xFFFF0000);
            img.SetPixel(Point{ 0, -1 }, 0xFFFF0000);
            img.SetPixel(Point{ 16, 0 }, 0xFFFF0000);
            img.SetPixel(Point{ 0, 16 }, 0xFFFF0000);

            REQUIRE(img.GetPixel(Point{ 0, 0 }) == originalColor);
        }
    }

    SECTION("Pixel coordinates within tile boundaries") 
    {
        SECTION("Pixel at tile boundaries") 
        {
            img.SetPixel(Point{ 7, 7 }, 0x88888888); 
            img.SetPixel(Point{ 8, 8 }, 0x99999999); 

            REQUIRE(img.GetPixel(Point{ 7, 7 }) == 0x88888888);
            REQUIRE(img.GetPixel(Point{ 8, 8 }) == 0x99999999);
        }

        SECTION("Pixel coordinate transformation") 
        {
            img.SetPixel(Point{ 9, 10 }, 0xAAAAAAAA);
            REQUIRE(img.GetPixel(Point{ 9, 10 }) == 0xAAAAAAAA);
        }
    }
}

TEST_CASE("Image Copy-on-Write Optimization") {
    SECTION("Copy constructor shares tiles initially") 
    {
        Image original(Size{ 16, 16 }, 0x12345678);

        SECTION("After copy, tiles are shared") 
        {
            Image copy = original;
            for (int y = 0; y < 16; ++y) 
            {
                for (int x = 0; x < 16; ++x) 
                {
                    REQUIRE(original.GetPixel(Point{ x, y }) == copy.GetPixel(Point{ x, y }));
                }
            }
        }
    }

    SECTION("Writing to copy triggers CoW") 
    {
        Image original(Size{ 16, 16 }, 0x11111111);
        Image copy = original;

        auto originalPixel = original.GetPixel(Point{ 5, 5 });

        SECTION("Modifying copy doesn't affect original") 
        {
            copy.SetPixel(Point{ 5, 5 }, 0x22222222);

            REQUIRE(original.GetPixel(Point{ 5, 5 }) == originalPixel);
            REQUIRE(copy.GetPixel(Point{ 5, 5 }) == 0x22222222);
        }

        SECTION("Modifying original doesn't affect copy") 
        {
            original.SetPixel(Point{ 5, 5 }, 0x33333333);

            REQUIRE(copy.GetPixel(Point{ 5, 5 }) == originalPixel);
            REQUIRE(original.GetPixel(Point{ 5, 5 }) == 0x33333333);
        }
    }

    SECTION("Multiple copies and modifications") 
    {
        Image original(Size{ 16, 16 }, 0x44444444);
        Image copy1 = original;
        Image copy2 = original;

        SECTION("Independent modifications")
        {
            original.SetPixel(Point{ 1, 1 }, 0x55555555);
            copy1.SetPixel(Point{ 2, 2 }, 0x66666666);
            copy2.SetPixel(Point{ 3, 3 }, 0x77777777);

            REQUIRE(original.GetPixel(Point{ 1, 1 }) == 0x55555555);
            REQUIRE(original.GetPixel(Point{ 2, 2 }) == 0x44444444);
            REQUIRE(original.GetPixel(Point{ 3, 3 }) == 0x44444444);

            REQUIRE(copy1.GetPixel(Point{ 1, 1 }) == 0x44444444);
            REQUIRE(copy1.GetPixel(Point{ 2, 2 }) == 0x66666666);
            REQUIRE(copy1.GetPixel(Point{ 3, 3 }) == 0x44444444);

            REQUIRE(copy2.GetPixel(Point{ 1, 1 }) == 0x44444444);
            REQUIRE(copy2.GetPixel(Point{ 2, 2 }) == 0x44444444);
            REQUIRE(copy2.GetPixel(Point{ 3, 3 }) == 0x77777777);
        }

        SECTION("Modifying different tiles in copies") 
        {
            copy1.SetPixel(Point{ 5, 5 }, 0x88888888);   
            copy2.SetPixel(Point{ 12, 12 }, 0x99999999); 

            // Оригинал остается неизменным
            REQUIRE(original.GetPixel(Point{ 5, 5 }) == 0x44444444);
            REQUIRE(original.GetPixel(Point{ 12, 12 }) == 0x44444444);

            // Каждая копия имеет свои изменения
            REQUIRE(copy1.GetPixel(Point{ 5, 5 }) == 0x88888888);
            REQUIRE(copy2.GetPixel(Point{ 12, 12 }) == 0x99999999);

            // Неизмененные пиксели остаются общими
            REQUIRE(original.GetPixel(Point{ 0, 0 }) == copy1.GetPixel(Point{ 0, 0 }));
            REQUIRE(original.GetPixel(Point{ 0, 0 }) == copy2.GetPixel(Point{ 0, 0 }));
        }
    }

    SECTION("CoW efficiency - tiles are copied only when necessary")
    {
        Image original(Size{ 24, 24 }, 0xAAAAAAA); 

        SECTION("Multiple modifications in same tile")
        {
            Image copy = original;

            copy.SetPixel(Point{ 1, 1 }, 0xBBBBBBBB);
            copy.SetPixel(Point{ 2, 2 }, 0xCCCCCCCC);
            copy.SetPixel(Point{ 3, 3 }, 0xDDDDDDDD);

            // Все изменения в тайле (0,0) должны использовать одну копию
            REQUIRE(copy.GetPixel(Point{ 1, 1 }) == 0xBBBBBBBB);
            REQUIRE(copy.GetPixel(Point{ 2, 2 }) == 0xCCCCCCCC);
            REQUIRE(copy.GetPixel(Point{ 3, 3 }) == 0xDDDDDDDD);

            // Оригинал не изменился
            REQUIRE(original.GetPixel(Point{ 1, 1 }) == 0xAAAAAAA);
        }
    }
}

TEST_CASE("Image Edge Cases") 
{
    SECTION("Single pixel image") 
    {
        Image img(Size{ 1, 1 }, 0x12345678);
        REQUIRE(img.GetSize().width == 1);
        REQUIRE(img.GetSize().height == 1);
        REQUIRE(img.GetPixel(Point{ 0, 0 }) == 0x12345678);

        img.SetPixel(Point{ 0, 0 }, 0x87654321);
        REQUIRE(img.GetPixel(Point{ 0, 0 }) == 0x87654321);
    }

    SECTION("Large image") 
    {
        Image img(Size{ 100, 100 }, 0x55555555);

        // Проверяем различные области
        REQUIRE(img.GetPixel(Point{ 0, 0 }) == 0x55555555);
        REQUIRE(img.GetPixel(Point{ 99, 99 }) == 0x55555555);
        REQUIRE(img.GetPixel(Point{ 50, 50 }) == 0x55555555);

        // Изменяем и проверяем
        img.SetPixel(Point{ 50, 50 }, 0x66666666);
        REQUIRE(img.GetPixel(Point{ 50, 50 }) == 0x66666666);
    }

    SECTION("Image with size just above tile boundary") 
    {
        Image img(Size{ 9, 9 }, 0x77777777); // Требует 2x2 тайла

        REQUIRE(img.GetSize().width == 9);
        REQUIRE(img.GetSize().height == 9);

        // Проверяем граничные пиксели
        REQUIRE(img.GetPixel(Point{ 8, 8 }) == 0x77777777);

        // Изменяем пиксель в "лишнем" пространстве
        img.SetPixel(Point{ 8, 8 }, 0x88888888);
        REQUIRE(img.GetPixel(Point{ 8, 8 }) == 0x88888888);
    }
}

TEST_CASE("Image Copy-on-Write with Tile Instance Counting")
{
    int initialTileCount = Tile::GetInstanceCount();

    SECTION("Single tile image CoW behavior")
    {
        Image img(Size{ 8, 8 }, 0x11111111);
        REQUIRE(Tile::GetInstanceCount() == initialTileCount + 1); 

        SECTION("Copy shares the same tile") {
            Image copy = img;
            REQUIRE(Tile::GetInstanceCount() == initialTileCount + 1); 

            SECTION("Modifying copy creates new tile") {
                copy.SetPixel(Point{ 0, 0 }, 0x22222222);
                REQUIRE(Tile::GetInstanceCount() == initialTileCount + 2); 

                // Проверяем, что оригинал не изменился
                REQUIRE(img.GetPixel(Point{ 0, 0 }) == 0x11111111);
                REQUIRE(copy.GetPixel(Point{ 0, 0 }) == 0x22222222);
            }
        }
    }
}