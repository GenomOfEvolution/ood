#pragma once
#include <catch.hpp>

#include "../ShapesApp/Models/DocumentItem/DocumentItem.h"
#include "../ShapesApp/Models/DocumentItem/Shape/Ellipse/CEllipse.h"
#include "../ShapesApp/Models/DocumentItem/Shape/Rectangle/CRectangle.h"
#include "../ShapesApp/Models/DocumentItem/Shape/Triangle/CTriangle.h"
#include "../ShapesApp/Models/DocumentItem/Image/CImage.h"

TEST_CASE("DocumentItem methods")
{
    GIVEN("A DocumentItem containing an image")
    {
        auto image = std::make_shared<CImage>("test.png", Point(0, 0), 100, 50);
        DocumentItem docItem(std::static_pointer_cast<IImage>(image));

        WHEN("Getting image and shape pointers")
        {
            THEN("GetImage returns valid pointer, GetShape returns nullptr")
            {
                REQUIRE(docItem.GetImage() != nullptr);
                REQUIRE(docItem.GetShape() == nullptr);
                REQUIRE(docItem.GetPreview().m_type == DocItemPreview::ItemType::Image);
            }
        }

        WHEN("Checking point containment")
        {
            THEN("Behaves like the underlying image")
            {
                REQUIRE(docItem.ContainsPoint(Point(50, 25)) == true);  // Inside
                REQUIRE(docItem.ContainsPoint(Point(-7, 25)) == true);  // Border area
                REQUIRE(docItem.ContainsPoint(Point(-8, 25)) == false); // Outside border
            }
        }

        WHEN("Moving the item by (10, 15)")
        {
            docItem.MoveBy(Point(10, 15));
            Rect bbox = docItem.GetBoundingBox();

            THEN("Position updates correctly")
            {
                REQUIRE(bbox.x == Approx(10.0));
                REQUIRE(bbox.y == Approx(15.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(50.0));
            }
        }

        WHEN("Resizing with BottomRight handle (+20, +10)")
        {
            docItem.Resize(HandleType::BottomRight, 20, 10);
            Rect bbox = docItem.GetBoundingBox();

            THEN("Size increases correctly")
            {
                REQUIRE(bbox.width == Approx(120.0));
                REQUIRE(bbox.height == Approx(60.0));
            }
        }

        WHEN("Resizing with TopLeft handle (-10, -5)")
        {
            docItem.Resize(HandleType::TopLeft, -10, -5);
            Rect bbox = docItem.GetBoundingBox();

            THEN("Position and size update correctly")
            {
                REQUIRE(bbox.x == Approx(-10.0));
                REQUIRE(bbox.y == Approx(-5.0));
                REQUIRE(bbox.width == Approx(110.0));
                REQUIRE(bbox.height == Approx(55.0));
            }
        }

        WHEN("Resizing below minimum size with BottomRight handle")
        {
            docItem.Resize(HandleType::BottomRight, -90, -40); // Trying to make width=10, height=10
            Rect bbox = docItem.GetBoundingBox();

            THEN("Dimensions are clamped to minimum size (20x20)")
            {
                REQUIRE(bbox.width == Approx(20.0));
                REQUIRE(bbox.height == Approx(20.0));
            }
        }

        WHEN("Resizing to explicit bounding box (30, 40, 70, 35)")
        {
            docItem.Resize(Rect(30, 40, 70, 35));
            Rect bbox = docItem.GetBoundingBox();

            THEN("Dimensions and position update exactly")
            {
                REQUIRE(bbox.x == Approx(30.0));
                REQUIRE(bbox.y == Approx(40.0));
                REQUIRE(bbox.width == Approx(70.0));
                REQUIRE(bbox.height == Approx(35.0));
            }
        }
    }

    GIVEN("A DocumentItem containing a rectangle")
    {
        auto rect = std::make_shared<CRectangle>(Point(20, 30), 80, 60);
        DocumentItem docItem(rect);

        WHEN("Getting image and shape pointers")
        {
            THEN("GetShape returns valid pointer, GetImage returns nullptr")
            {
                REQUIRE(docItem.GetShape() != nullptr);
                REQUIRE(docItem.GetImage() == nullptr);
                REQUIRE(docItem.GetPreview().m_type == DocItemPreview::ItemType::Rectangle);
            }
        }

        WHEN("Getting bounding box")
        {
            Rect bbox = docItem.GetBoundingBox();
            THEN("Matches rectangle dimensions")
            {
                REQUIRE(bbox.x == Approx(20.0));
                REQUIRE(bbox.y == Approx(30.0));
                REQUIRE(bbox.width == Approx(80.0));
                REQUIRE(bbox.height == Approx(60.0));
            }
        }

        WHEN("Resizing with BottomLeft handle (-15, +25)")
        {
            docItem.Resize(HandleType::BottomLeft, -15, 25);
            Rect bbox = docItem.GetBoundingBox();

            THEN("Position and dimensions update correctly")
            {
                REQUIRE(bbox.x == Approx(5.0));   // 20 - 15
                REQUIRE(bbox.width == Approx(95.0)); // 80 + 15
                REQUIRE(bbox.height == Approx(85.0)); // 60 + 25
            }
        }
    }

    GIVEN("A DocumentItem containing an ellipse")
    {
        auto ellipse = std::make_shared<CEllipse>(Point(50, 50), 30, 40);
        DocumentItem docItem(ellipse);

        WHEN("Resizing with handles")
        {
            docItem.Resize(HandleType::BottomRight, 20, 10);
            Rect bbox = docItem.GetBoundingBox();

            THEN("Ellipse bounding box updates correctly")
            {
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(70.0));  
            }
        }
    }

    GIVEN("A DocumentItem containing a triangle")
    {
        auto triangle = std::make_shared<CTriangle>(
            Point(0, 100),
            Point(100, 100),
            Point(50, 0)
        );
        DocumentItem docItem(triangle);

        WHEN("Resizing with TopLeft handle")
        {
            docItem.Resize(HandleType::TopLeft, -20, -20);
            auto points = docItem.GetShape()->GetBoundingBox();

            THEN("Triangle vertices scale proportionally")
            {
                Rect bbox = docItem.GetBoundingBox();
                REQUIRE(bbox.width == Approx(120.0));
                REQUIRE(bbox.height == Approx(120.0));
            }
        }
    }
}