#pragma once
#include <catch.hpp>

#include "../ShapesApp/Models/DocumentItem/Shape/Ellipse/CEllipse.h"
#include "../ShapesApp/Models/DocumentItem/Shape/Rectangle/CRectangle.h"
#include "../ShapesApp/Models/DocumentItem/Shape/Triangle/CTriangle.h"
#include "../ShapesApp/Models/DocumentItem/Image/CImage.h"

TEST_CASE("CRectangle methods")
{
    GIVEN("A rectangle at (0, 0) with width 100 and height 50")
    {
        CRectangle rect(Point(0, 0), 100, 50);

        WHEN("Checking point containment")
        {
            THEN("Contains points inside and on edges")
            {
                REQUIRE(rect.ContainsPoint(Point(50, 25)) == true);    // Center point
                REQUIRE(rect.ContainsPoint(Point(0, 25)) == true);     // Left edge
                REQUIRE(rect.ContainsPoint(Point(100, 25)) == true);   // Right edge
                REQUIRE(rect.ContainsPoint(Point(50, 0)) == true);     // Top edge
                REQUIRE(rect.ContainsPoint(Point(50, 50)) == true);    // Bottom edge
                REQUIRE(rect.ContainsPoint(Point(0, 0)) == true);      // Top-left corner
                REQUIRE(rect.ContainsPoint(Point(100, 50)) == true);   // Bottom-right corner
            }

            THEN("Contains points near the border (within borderWidth)")
            {
                // Points just outside the rectangle but within the border width (15px)
                REQUIRE(rect.ContainsPoint(Point(-7, 25)) == true);     // Left border area (within 7.5px)
                REQUIRE(rect.ContainsPoint(Point(107, 25)) == true);    // Right border area (within 7.5px)
                REQUIRE(rect.ContainsPoint(Point(50, -7)) == true);     // Top border area (within 7.5px)
                REQUIRE(rect.ContainsPoint(Point(50, 57)) == true);     // Bottom border area (within 7.5px)

                // Corner border areas
                REQUIRE(rect.ContainsPoint(Point(-7, -7)) == true);     // Top-left corner border
                REQUIRE(rect.ContainsPoint(Point(107, 57)) == true);    // Bottom-right corner border
            }

            THEN("Does not contain points far outside the border")
            {
                // Points beyond the border width (15px)
                REQUIRE(rect.ContainsPoint(Point(-8, 25)) == false);    // Left outside border area
                REQUIRE(rect.ContainsPoint(Point(108, 25)) == false);   // Right outside border area
                REQUIRE(rect.ContainsPoint(Point(50, -8)) == false);    // Top outside border area
                REQUIRE(rect.ContainsPoint(Point(50, 58)) == false);    // Bottom outside border area

                // Far corner points
                REQUIRE(rect.ContainsPoint(Point(-20, -20)) == false);  // Far top-left
                REQUIRE(rect.ContainsPoint(Point(120, 60)) == false);   // Far bottom-right
            }
        }

        WHEN("Getting bounding box")
        {
            Rect bbox = rect.GetBoundingBox();
            THEN("Bounding box matches rectangle dimensions")
            {
                REQUIRE(bbox.x == Approx(0.0));
                REQUIRE(bbox.y == Approx(0.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(50.0));
            }
        }

        WHEN("Moving the rectangle by (10, 20)")
        {
            rect.MoveBy(Point(10, 20));
            Rect bbox = rect.GetBoundingBox();
            THEN("Position updates correctly")
            {
                REQUIRE(bbox.x == Approx(10.0));
                REQUIRE(bbox.y == Approx(20.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(50.0));
            }
        }

        WHEN("Resizing to new bounding box (25, 25, 50, 25)")
        {
            rect.Resize(Rect(25, 25, 50, 25));
            Rect bbox = rect.GetBoundingBox();
            THEN("Dimensions and position update correctly")
            {
                REQUIRE(bbox.x == Approx(25.0));
                REQUIRE(bbox.y == Approx(25.0));
                REQUIRE(bbox.width == Approx(50.0));
                REQUIRE(bbox.height == Approx(25.0));
            }
        }

        WHEN("Getting preview")
        {
            DocItemPreview preview = rect.GetPreview();
            THEN("Preview has correct type and dimensions")
            {
                REQUIRE(preview.m_type == DocItemPreview::ItemType::Rectangle);
                REQUIRE(preview.m_boundingBox.x == Approx(0.0));
                REQUIRE(preview.m_boundingBox.y == Approx(0.0));
                REQUIRE(preview.m_boundingBox.width == Approx(100.0));
                REQUIRE(preview.m_boundingBox.height == Approx(50.0));
            }
        }
    }
}

TEST_CASE("CEllipse methods")
{
    GIVEN("An ellipse centered at (50, 30) with vertical radius 20 and horizontal radius 40")
    {
        CEllipse ellipse(Point(50, 30), 20, 40);

        WHEN("Checking point containment")
        {
            THEN("Contains points inside the ellipse")
            {
                REQUIRE(ellipse.ContainsPoint(Point(50, 30)) == true);      // Center point
                REQUIRE(ellipse.ContainsPoint(Point(50, 40)) == true);      // Top semi-minor axis
                REQUIRE(ellipse.ContainsPoint(Point(70, 30)) == true);      // Right semi-major axis
                REQUIRE(ellipse.ContainsPoint(Point(50 + 28.28, 30 + 14.14)) == true); // 45 degrees point (40/sqrt(2), 20/sqrt(2))
            }

            THEN("Contains points on the ellipse boundary")
            {
                REQUIRE(ellipse.ContainsPoint(Point(90, 30)) == true);      // Rightmost point
                REQUIRE(ellipse.ContainsPoint(Point(10, 30)) == true);      // Leftmost point
                REQUIRE(ellipse.ContainsPoint(Point(50, 50)) == true);      // Bottommost point
                REQUIRE(ellipse.ContainsPoint(Point(50, 10)) == true);      // Topmost point
            }

            THEN("Contains points near the border (within borderWidth)")
            {
                Rect bbox = ellipse.GetBoundingBox();
                // Points just outside the bounding box but within border width (15px)
                REQUIRE(ellipse.ContainsPoint(Point(bbox.x - 7, 30)) == true);     // Left border area
                REQUIRE(ellipse.ContainsPoint(Point(bbox.x + bbox.width + 7, 30)) == true);  // Right border area
                REQUIRE(ellipse.ContainsPoint(Point(50, bbox.y - 7)) == true);     // Top border area
                REQUIRE(ellipse.ContainsPoint(Point(50, bbox.y + bbox.height + 7)) == true); // Bottom border area

                // Corner border areas
                REQUIRE(ellipse.ContainsPoint(Point(bbox.x - 7, bbox.y - 7)) == true); // Top-left corner border
                REQUIRE(ellipse.ContainsPoint(Point(bbox.x + bbox.width + 7, bbox.y + bbox.height + 7)) == true); // Bottom-right corner border
            }

            THEN("Does not contain points far outside the border")
            {
                Rect bbox = ellipse.GetBoundingBox();
                // Points beyond the border width (15px)
                REQUIRE(ellipse.ContainsPoint(Point(bbox.x - 8, 30)) == false);    // Left outside border
                REQUIRE(ellipse.ContainsPoint(Point(bbox.x + bbox.width + 8, 30)) == false); // Right outside border
                REQUIRE(ellipse.ContainsPoint(Point(50, bbox.y - 8)) == false);    // Top outside border
                REQUIRE(ellipse.ContainsPoint(Point(50, bbox.y + bbox.height + 8)) == false); // Bottom outside border

                // Far points
                REQUIRE(ellipse.ContainsPoint(Point(200, 200)) == false);
                REQUIRE(ellipse.ContainsPoint(Point(-50, -50)) == false);
            }
        }

        WHEN("Getting bounding box")
        {
            Rect bbox = ellipse.GetBoundingBox();
            THEN("Bounding box dimensions are correct")
            {
                REQUIRE(bbox.x == Approx(10.0));         // 50 - 40
                REQUIRE(bbox.y == Approx(10.0));         // 30 - 20
                REQUIRE(bbox.width == Approx(80.0));     // 40 * 2
                REQUIRE(bbox.height == Approx(40.0));    // 20 * 2
            }
        }

        WHEN("Moving the ellipse by (10, 5)")
        {
            ellipse.MoveBy(Point(10, 5));
            Rect bbox = ellipse.GetBoundingBox();
            THEN("Position updates correctly")
            {
                REQUIRE(bbox.x == Approx(20.0));
                REQUIRE(bbox.y == Approx(15.0));
                REQUIRE(bbox.width == Approx(80.0));
                REQUIRE(bbox.height == Approx(40.0));
                REQUIRE(ellipse.GetCenter().x == Approx(60.0));
                REQUIRE(ellipse.GetCenter().y == Approx(35.0));
            }
        }

        WHEN("Resizing to new bounding box (20, 20, 100, 60)")
        {
            ellipse.Resize(Rect(20, 20, 100, 60));
            Rect bbox = ellipse.GetBoundingBox();
            THEN("Dimensions and position update correctly")
            {
                REQUIRE(bbox.x == Approx(20.0));
                REQUIRE(bbox.y == Approx(20.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(60.0));
                REQUIRE(ellipse.GetCenter().x == Approx(70.0));   // 20 + 100/2
                REQUIRE(ellipse.GetCenter().y == Approx(50.0));   // 20 + 60/2
                REQUIRE(ellipse.GetHorizRadius() == Approx(50.0)); // 100/2
                REQUIRE(ellipse.GetVertRadius() == Approx(30.0));  // 60/2
            }
        }

        WHEN("Getting preview")
        {
            DocItemPreview preview = ellipse.GetPreview();
            THEN("Preview has correct type and properties")
            {
                REQUIRE(preview.m_type == DocItemPreview::ItemType::Ellipse);
                REQUIRE(preview.m_points.size() == 1);
                REQUIRE(preview.m_points[0].x == Approx(50.0));
                REQUIRE(preview.m_points[0].y == Approx(30.0));

                Rect bbox = preview.m_boundingBox;
                REQUIRE(bbox.x == Approx(10.0));
                REQUIRE(bbox.y == Approx(10.0));
                REQUIRE(bbox.width == Approx(80.0));
                REQUIRE(bbox.height == Approx(40.0));
            }
        }
    }
}

TEST_CASE("CTriangle methods")
{
    GIVEN("A triangle with vertices at (0, 0), (100, 0), and (50, 100)")
    {
        CTriangle triangle(Point(0, 0), Point(100, 0), Point(50, 100));

        WHEN("Checking point containment")
        {
            THEN("Contains points inside the triangle")
            {
                REQUIRE(triangle.ContainsPoint(Point(50, 30)) == true);    // Center point
                REQUIRE(triangle.ContainsPoint(Point(25, 25)) == true);    // Left side
                REQUIRE(triangle.ContainsPoint(Point(75, 25)) == true);    // Right side
            }

            THEN("Contains points on the edges")
            {
                REQUIRE(triangle.ContainsPoint(Point(50, 0)) == true);     // Bottom edge center
                REQUIRE(triangle.ContainsPoint(Point(25, 50)) == true);    // Left edge
                REQUIRE(triangle.ContainsPoint(Point(75, 50)) == true);    // Right edge
                REQUIRE(triangle.ContainsPoint(Point(0, 0)) == true);      // Vertex 1
                REQUIRE(triangle.ContainsPoint(Point(100, 0)) == true);    // Vertex 2
                REQUIRE(triangle.ContainsPoint(Point(50, 100)) == true);   // Vertex 3
            }

            THEN("Contains points near the border (within borderWidth)")
            {
                Rect bbox = triangle.GetBoundingBox();
                // Points just outside the triangle but within border width (15px)
                REQUIRE(triangle.ContainsPoint(Point(-7, 50)) == true);    // Left border area
                REQUIRE(triangle.ContainsPoint(Point(107, 50)) == true);   // Right border area
                REQUIRE(triangle.ContainsPoint(Point(50, -7)) == true);    // Bottom border area
                REQUIRE(triangle.ContainsPoint(Point(50, 107)) == true);   // Top border area
            }

            THEN("Does not contain points far outside the border")
            {
                Rect bbox = triangle.GetBoundingBox();
                REQUIRE(triangle.ContainsPoint(Point(-8, 50)) == false);   // Left outside border
                REQUIRE(triangle.ContainsPoint(Point(108, 50)) == false);  // Right outside border
                REQUIRE(triangle.ContainsPoint(Point(50, -8)) == false);   // Bottom outside border
                REQUIRE(triangle.ContainsPoint(Point(50, 108)) == false);  // Top outside border
                REQUIRE(triangle.ContainsPoint(Point(200, 200)) == false); // Far outside
            }
        }

        WHEN("Getting bounding box")
        {
            Rect bbox = triangle.GetBoundingBox();
            THEN("Bounding box dimensions are correct")
            {
                REQUIRE(bbox.x == Approx(0.0));
                REQUIRE(bbox.y == Approx(0.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(100.0));
            }
        }

        WHEN("Moving the triangle by (10, 20)")
        {
            triangle.MoveBy(Point(10, 20));
            auto points = triangle.GetPoints();
            Rect bbox = triangle.GetBoundingBox();

            THEN("All vertices move correctly")
            {
                REQUIRE(points[0].x == Approx(10.0));
                REQUIRE(points[0].y == Approx(20.0));
                REQUIRE(points[1].x == Approx(110.0));
                REQUIRE(points[1].y == Approx(20.0));
                REQUIRE(points[2].x == Approx(60.0));
                REQUIRE(points[2].y == Approx(120.0));

                REQUIRE(bbox.x == Approx(10.0));
                REQUIRE(bbox.y == Approx(20.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(100.0));
            }
        }

        WHEN("Resizing to new bounding box (20, 30, 80, 60)")
        {
            triangle.Resize(Rect(20, 30, 80, 60));
            auto points = triangle.GetPoints();
            Rect bbox = triangle.GetBoundingBox();

            THEN("Vertices are scaled proportionally within new bounding box")
            {
                REQUIRE(bbox.x == Approx(20.0));
                REQUIRE(bbox.y == Approx(30.0));
                REQUIRE(bbox.width == Approx(80.0));
                REQUIRE(bbox.height == Approx(60.0));

                REQUIRE(points[0].x == Approx(20.0));
                REQUIRE(points[0].y == Approx(30.0));

                REQUIRE(points[1].x == Approx(100.0));
                REQUIRE(points[1].y == Approx(30.0));

                REQUIRE(points[2].x == Approx(60.0));
                REQUIRE(points[2].y == Approx(90.0));
            }
        }

        WHEN("Getting preview")
        {
            DocItemPreview preview = triangle.GetPreview();
            THEN("Preview has correct type and properties")
            {
                REQUIRE(preview.m_type == DocItemPreview::ItemType::Triangle);
                REQUIRE(preview.m_points.size() == 3);

                REQUIRE(preview.m_points[0].x == Approx(0.0));
                REQUIRE(preview.m_points[0].y == Approx(0.0));
                REQUIRE(preview.m_points[1].x == Approx(100.0));
                REQUIRE(preview.m_points[1].y == Approx(0.0));
                REQUIRE(preview.m_points[2].x == Approx(50.0));
                REQUIRE(preview.m_points[2].y == Approx(100.0));

                Rect bbox = preview.m_boundingBox;
                REQUIRE(bbox.x == Approx(0.0));
                REQUIRE(bbox.y == Approx(0.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(100.0));
            }
        }

        WHEN("Getting triangle points")
        {
            auto points = triangle.GetPoints();
            THEN("Points match the original vertices")
            {
                REQUIRE(points[0].x == Approx(0.0));
                REQUIRE(points[0].y == Approx(0.0));
                REQUIRE(points[1].x == Approx(100.0));
                REQUIRE(points[1].y == Approx(0.0));
                REQUIRE(points[2].x == Approx(50.0));
                REQUIRE(points[2].y == Approx(100.0));
            }
        }
    }
}

TEST_CASE("CImage methods")
{
    GIVEN("An image at (0, 0) with width 100 and height 50, path \"test.png\"")
    {
        CImage image("test.png", Point(0, 0), 100, 50);

        WHEN("Getting and setting path")
        {
            THEN("Initial path is correct")
            {
                REQUIRE(image.GetPath() == "test.png");
            }

            AND_WHEN("Path is changed to \"new_image.jpg\"")
            {
                image.SetPath("new_image.jpg");
                THEN("Path updates correctly")
                {
                    REQUIRE(image.GetPath() == "new_image.jpg");
                }
            }
        }

        WHEN("Checking point containment")
        {
            THEN("Contains points inside and on edges")
            {
                REQUIRE(image.ContainsPoint(Point(50, 25)) == true);    // Center point
                REQUIRE(image.ContainsPoint(Point(0, 25)) == true);     // Left edge
                REQUIRE(image.ContainsPoint(Point(100, 25)) == true);   // Right edge
                REQUIRE(image.ContainsPoint(Point(50, 0)) == true);     // Top edge
                REQUIRE(image.ContainsPoint(Point(50, 50)) == true);    // Bottom edge
                REQUIRE(image.ContainsPoint(Point(0, 0)) == true);      // Top-left corner
                REQUIRE(image.ContainsPoint(Point(100, 50)) == true);   // Bottom-right corner
            }

            THEN("Contains points near the border (within borderWidth)")
            {
                REQUIRE(image.ContainsPoint(Point(-7, 25)) == true);    // Left border area
                REQUIRE(image.ContainsPoint(Point(107, 25)) == true);   // Right border area
                REQUIRE(image.ContainsPoint(Point(50, -7)) == true);    // Top border area
                REQUIRE(image.ContainsPoint(Point(50, 57)) == true);    // Bottom border area
            }

            THEN("Does not contain points far outside the border")
            {
                REQUIRE(image.ContainsPoint(Point(-8, 25)) == false);   // Left outside border
                REQUIRE(image.ContainsPoint(Point(108, 25)) == false);  // Right outside border
                REQUIRE(image.ContainsPoint(Point(50, -8)) == false);   // Top outside border
                REQUIRE(image.ContainsPoint(Point(50, 58)) == false);   // Bottom outside border
            }
        }

        WHEN("Getting bounding box")
        {
            Rect bbox = image.GetBoundingBox();
            THEN("Bounding box matches image dimensions")
            {
                REQUIRE(bbox.x == Approx(0.0));
                REQUIRE(bbox.y == Approx(0.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(50.0));
            }
        }

        WHEN("Moving the image by (15, 25)")
        {
            image.MoveBy(Point(15, 25));
            Rect bbox = image.GetBoundingBox();
            THEN("Position updates correctly")
            {
                REQUIRE(bbox.x == Approx(15.0));
                REQUIRE(bbox.y == Approx(25.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(50.0));
            }
        }

        WHEN("Resizing to new bounding box (30, 40, 60, 30)")
        {
            image.Resize(Rect(30, 40, 60, 30));
            Rect bbox = image.GetBoundingBox();
            THEN("Dimensions and position update correctly")
            {
                REQUIRE(bbox.x == Approx(30.0));
                REQUIRE(bbox.y == Approx(40.0));
                REQUIRE(bbox.width == Approx(60.0));
                REQUIRE(bbox.height == Approx(30.0));
            }
        }

        WHEN("Getting preview")
        {
            DocItemPreview preview = image.GetPreview();
            THEN("Preview has correct type, bounding box and image path")
            {
                REQUIRE(preview.m_type == DocItemPreview::ItemType::Image);
                REQUIRE(preview.m_imgPath == "test.png");

                Rect bbox = preview.m_boundingBox;
                REQUIRE(bbox.x == Approx(0.0));
                REQUIRE(bbox.y == Approx(0.0));
                REQUIRE(bbox.width == Approx(100.0));
                REQUIRE(bbox.height == Approx(50.0));
            }
        }
    }
}