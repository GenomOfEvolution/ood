#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <fakeit.hpp>

#include "../Designer/Canvas/ICanvas.h"
#include "../Designer/Shapes/ShapeTypes.h"
#include "../Designer/Shapes/Ellipse/Ellipse.h"
#include "../Designer/Shapes/Rectangle/Rectangle.h"
#include "../Designer/Shapes/RegularPolygon/RegularPolygon.h"
#include "../Designer/Shapes/Triangle/Triangle.h"

TEST_CASE("Shapes")
{
	fakeit::Mock<ICanvas> mockCanvas;
	fakeit::Fake(Method(mockCanvas, SetColor));
	fakeit::Fake(Method(mockCanvas, DrawLine));
	fakeit::Fake(Method(mockCanvas, DrawEllipse));

	auto& canvas = mockCanvas.get();

	SECTION("Regular Polygon")
	{
		GIVEN("color, amount of vertices, radius, center point")
		{
			const Color color = Color::Yellow;
			const size_t verticesCount = 8;
			const Point center = { 100, 200 };
			const double radius = 20;

			WHEN("creating an polygon")
			{
				RegularPolygon polygon(color, verticesCount, radius, center);
				THEN("it creates correctly")
				{
					REQUIRE(polygon.GetCenter() == center);
					REQUIRE(polygon.GetColor() == color);
					REQUIRE(polygon.GetRadius() == radius);
					REQUIRE(polygon.GetVertexCount() == verticesCount);
				}

				AND_WHEN("Drawing it")
				{
					polygon.Draw(canvas);

					THEN("it draws correctly")
					{
						fakeit::Verify(Method(mockCanvas, SetColor).Using(color));
						fakeit::Verify(Method(mockCanvas, DrawLine)).Exactly(verticesCount);
					}
				}
			}
		}

		GIVEN("invalid vertex count")
		{
			std::size_t const vertexCount = 1;

			WHEN("creating a regular polygon")
			{
				THEN("it throws an exception")
				{
					REQUIRE_THROWS_AS(RegularPolygon(Color::Yellow, vertexCount, 1, { 1, 1 }), std::invalid_argument);
				}
			}
		}

		GIVEN("minimal vertex count")
		{
			std::size_t const vertexCount = 2;
			WHEN("creating a regular polygon")
			{
				THEN("it doesn't throw an exception")
				{
					REQUIRE_NOTHROW(RegularPolygon(Color::Yellow, vertexCount, 1, { 1, 1 }));
				}

				AND_THEN("it has propper vertex count")
				{
					RegularPolygon polygon(Color::Yellow, vertexCount, 1, { 1, 1 });
					REQUIRE(polygon.GetVertexCount() == vertexCount);
				}
			}
		}
	}

	SECTION("Ellipse")
	{
		GIVEN("color, center point, vertical and horizontal radius")
		{
			const Color color = Color::Yellow;
			const Point center = { 100, 100 };
			const double vertRad = 124;
			const double horRad = 123;

			WHEN("creating an ellipse")
			{
				CEllipse ellipse(color, center, horRad, vertRad);

				THEN("it creates correctly")
				{
					REQUIRE(ellipse.GetCenter() == center);
					REQUIRE(ellipse.GetColor() == color);
					REQUIRE(ellipse.GetHorizontalRadius() == horRad);
					REQUIRE(ellipse.GetVerticalRadius() == vertRad);
				}

				AND_WHEN("Drawing it")
				{
					ellipse.Draw(canvas);

					THEN("it draws correctly")
					{
						fakeit::Verify(Method(mockCanvas, SetColor).Using(color));
						fakeit::Verify(Method(mockCanvas, DrawEllipse).Using(center, horRad, vertRad));
					}
				}
			}
		}
	}

	SECTION("rectangle")
	{
		GIVEN("color, left top corner, right bottom corner")
		{
			auto const color = Color::Yellow;
			Point const leftTop = { 100, 200 };
			const double width = 100;
			const double height = 200;
			Point const rightBottom = { leftTop.x + width, leftTop.y + height };

			WHEN("creating a rectangle")
			{
				CRectangle rectangle(color, leftTop, width, height);

				THEN("it has correct parameters")
				{
					REQUIRE(rectangle.GetColor() == color);

					REQUIRE(rectangle.GetLeftTop() == leftTop);
					REQUIRE(rectangle.GetRightBottom() == rightBottom);
				}

				AND_WHEN("drawing the rectangle")
				{
					rectangle.Draw(canvas);

					THEN("it is drawn correctly")
					{
						fakeit::Verify(Method(mockCanvas, SetColor).Using(color));
						fakeit::Verify(Method(mockCanvas, DrawLine)).Exactly(4);
					}
				}
			}
		}
	}

	SECTION("Triangle")
	{
		GIVEN("color and 3 vertices")
		{
			auto const color = Color::Yellow;
			Point const vertex1 = { 100, 200 };
			Point const vertex2 = { 150, 250 };
			Point const vertex3 = { 100, 300 };

			WHEN("creating a triangle")
			{
				Triangle triangle(color, vertex1, vertex2, vertex3);

				THEN("it has correct parameters")
				{
					REQUIRE(triangle.GetColor() == color);

					REQUIRE(triangle.GetP1() == vertex1);
					REQUIRE(triangle.GetP2() == vertex2);
					REQUIRE(triangle.GetP3() == vertex3);
				}

				AND_WHEN("drawing the triangle")
				{
					triangle.Draw(canvas);

					THEN("it is drawn correctly")
					{
						fakeit::Verify(Method(mockCanvas, SetColor).Using(color));
						fakeit::Verify(Method(mockCanvas, DrawLine)).Exactly(3);
					}
				}
			}
		}
	}
}

TEST_CASE("shape factory")
{

}