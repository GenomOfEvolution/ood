#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <fakeit.hpp>

#include "../Designer/Canvas/ICanvas.h"
#include "../Designer/Shapes/ShapeTypes.h"
#include "../Designer/Shapes/Ellipse/Ellipse.h"
#include "../Designer/Shapes/Rectangle/Rectangle.h"
#include "../Designer/Shapes/RegularPolygon/RegularPolygon.h"
#include "../Designer/Shapes/Triangle/Triangle.h"
#include "../Designer/ShapeFactory/ShapeFactory.h"
#include "../Designer/PictureDraft/PictureDraft.h"
#include "../Designer/Designer/Designer.h"
#include "../Designer/Client/Client.h"
#include "../Designer/Painter/Painter.h"

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
						fakeit::Verify(Method(mockCanvas, DrawLine).Using(vertex1, vertex2)).Once();
						fakeit::Verify(Method(mockCanvas, DrawLine).Using(vertex2, vertex3)).Once();
						fakeit::Verify(Method(mockCanvas, DrawLine).Using(vertex1, vertex3)).Once();
					}
				}
			}
		}
	}
}

TEST_CASE("shape factory")
{
	GIVEN("an shape factory")
	{
		ShapeFactory factory;

		AND_GIVEN("valid ellipse description")
		{
			const std::string description = "ellipse pink 100 100 50 40";
			WHEN("creating a shape")
			{
				const auto shape = factory.CreateShape(description);
				THEN("it is a valid ellipse")
				{
					auto* ellipse = dynamic_cast<CEllipse*>(shape.get());
					REQUIRE(ellipse != nullptr);
					REQUIRE(ellipse->GetColor() == Color::Pink);
					REQUIRE(ellipse->GetCenter().x == 100);
					REQUIRE(ellipse->GetCenter().y == 100);
					REQUIRE(ellipse->GetHorizontalRadius() == 50);
					REQUIRE(ellipse->GetVerticalRadius() == 40);
				}
			}
		}

		AND_GIVEN("valid rectangle description")
		{
			const std::string description = "rectangle blue 50 60 100 80";
			WHEN("creating a shape")
			{
				const auto shape = factory.CreateShape(description);
				THEN("it is a valid rectangle")
				{
					auto* rectangle = dynamic_cast<CRectangle*>(shape.get());
					REQUIRE(rectangle != nullptr);
					REQUIRE(rectangle->GetColor() == Color::Blue);
					REQUIRE(rectangle->GetLeftTop().x == 50);
					REQUIRE(rectangle->GetLeftTop().y == 60);
					REQUIRE(rectangle->GetRightBottom().x == 150);
					REQUIRE(rectangle->GetRightBottom().y == 140);
				}
			}
		}
		
		AND_GIVEN("valid regular polygon description")
		{
			const std::string description = "regular-polygon green 5 75 200 150";
			WHEN("creating a shape")
			{
				const auto shape = factory.CreateShape(description);
				THEN("it is a valid regular polygon")
				{
					auto* polygon = dynamic_cast<RegularPolygon*>(shape.get());
					REQUIRE(polygon != nullptr);
					REQUIRE(polygon->GetColor() == Color::Green);
					REQUIRE(polygon->GetVertexCount() == 5);
					REQUIRE(polygon->GetRadius() == 75);
					REQUIRE(polygon->GetCenter().x == 200);
					REQUIRE(polygon->GetCenter().y == 150);
				}
			}
		}

		AND_GIVEN("valid triangle description")
		{
			const std::string description = "triangle yellow 10 20 50 60 30 80";
			WHEN("creating a shape")
			{
				const auto shape = factory.CreateShape(description);
				THEN("it is a valid triangle")
				{
					auto* triangle = dynamic_cast<Triangle*>(shape.get());
					REQUIRE(triangle != nullptr);
					REQUIRE(triangle->GetColor() == Color::Yellow);
					REQUIRE(triangle->GetP1().x == 10);
					REQUIRE(triangle->GetP1().y == 20);
					REQUIRE(triangle->GetP2().x == 50);
					REQUIRE(triangle->GetP2().y == 60);
					REQUIRE(triangle->GetP3().x == 30);
					REQUIRE(triangle->GetP3().y == 80);
				}
			}
		}

		AND_GIVEN("invalid shape description")
		{
			const std::string description = "invalid_shape red 10 20 30";
			WHEN("creating a shape")
			{
				THEN("it throws std::runtime_error")
				{
					REQUIRE_THROWS_AS(factory.CreateShape(description), std::runtime_error);
				}
			}
		}

		AND_GIVEN("invalid color in description")
		{
			const std::string description = "rectangle invalid_color 10 20 30 40";
			WHEN("creating a shape")
			{
				THEN("it throws std::runtime_error")
				{
					REQUIRE_THROWS_AS(factory.CreateShape(description), std::invalid_argument);
				}
			}
		}
	}
}

TEST_CASE("picture draft")
{
	GIVEN("an picture draft")
	{
		PictureDraft draft;

		WHEN("it just created")
		{
			THEN("it is empty")
			{
				REQUIRE(draft.GetShapeCount() == 0);
			}
		}

		WHEN("shapes are added")
		{
			auto shape1 = std::make_unique<CRectangle>(Color::Red, Point{ 10, 20 }, 30, 40);
			auto shape2 = std::make_unique<CEllipse>(Color::Blue, Point{ 50, 60 }, 70, 80);

			draft.AddShape(std::move(shape1));
			draft.AddShape(std::move(shape2));

			THEN("shape count increases and iteration works")
			{
				REQUIRE(draft.GetShapeCount() == 2);

				size_t count = 0;
				for (const auto& shape : draft) 
				{
					++count;
					REQUIRE(shape != nullptr);
				}
				REQUIRE(count == 2);

				count = 0;
				for (auto it = draft.cbegin(); it != draft.cend(); ++it) 
				{
					++count;
					REQUIRE(*it != nullptr);
				}
				REQUIRE(count == 2);
			}
		}

		WHEN("adding multiple shapes and checking order")
		{
			auto shape1 = std::make_unique<CRectangle>(Color::Red, Point{ 1, 1 }, 10, 10);
			auto shape2 = std::make_unique<CEllipse>(Color::Green, Point{ 2, 2 }, 20, 20);
			auto shape3 = std::make_unique<Triangle>(Color::Blue, Point{ 3, 3 }, Point{ 4, 4 }, Point{ 5, 5 });

			draft.AddShape(std::move(shape1));
			draft.AddShape(std::move(shape2));
			draft.AddShape(std::move(shape3));

			THEN("shapes are stored in the order they were added")
			{
				REQUIRE(draft.GetShapeCount() == 3);

				auto it = draft.begin();
				auto* firstShape = dynamic_cast<CRectangle*>((*it).get());
				REQUIRE(firstShape != nullptr);
				REQUIRE(firstShape->GetColor() == Color::Red);

				++it;
				auto* secondShape = dynamic_cast<CEllipse*>((*it).get());
				REQUIRE(secondShape != nullptr);
				REQUIRE(secondShape->GetColor() == Color::Green);

				++it;
				auto* thirdShape = dynamic_cast<Triangle*>((*it).get());
				REQUIRE(thirdShape != nullptr);
				REQUIRE(thirdShape->GetColor() == Color::Blue);
			}
		}
	}
}

TEST_CASE("designer")
{
	fakeit::Mock<IShapeFactory> factoryMock;
	fakeit::When(Method(factoryMock, CreateShape)).AlwaysDo([](const std::string&) -> std::unique_ptr<Shape> 
	{
		return std::make_unique<CEllipse>(Color::Pink, Point{ 100, 100 }, 12, 13);
	});

	auto& factory = factoryMock.get();

	GIVEN("a designer")
	{
		Designer designer(factory);

		WHEN("creating draft from empty stream")
		{
			std::istringstream emptyStream("");
			auto draft = designer.CreateDraft(emptyStream);

			THEN("draft is empty")
			{
				REQUIRE(draft.GetShapeCount() == 0);
			}

			THEN("no shapes are created")
			{
				fakeit::VerifyNoOtherInvocations(factoryMock);
			}
		}

		WHEN("creating draft with valid shape descriptions")
		{
			std::istringstream inputStream("ellipse pink 100 100 50 40\nrectangle blue 10 20 30 40\n");
			auto draft = designer.CreateDraft(inputStream);

			THEN("draft contains correct number of shapes")
			{
				REQUIRE(draft.GetShapeCount() == 2);
			}
		}

		WHEN("shape creation throws an exception")
		{
			fakeit::When(Method(factoryMock, CreateShape)).AlwaysThrow(std::invalid_argument("oops"));
			std::istringstream inputStream("poop");
			auto const draft = designer.CreateDraft(inputStream);

			THEN("draft is empty")
			{
				REQUIRE(draft.GetShapeCount() == 0);
			}
		}
	}
}
