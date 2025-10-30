#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <fakeit.hpp>

#include "../Slides/Canvas/ICanvas.h"
#include "../Slides/ShapeGroup/GroupShape.h"
#include "../Slides/Shape/Rectangle/Rectangle.h"
#include "../Slides/Shape/Ellipse/Ellipse.h"

TEST_CASE("Group Shape")
{
	fakeit::Mock<ICanvas> canvasMock;
	fakeit::Fake(Method(canvasMock, DrawPolygon));
	fakeit::Fake(Method(canvasMock, DrawEllipse));

	fakeit::Fake(Method(canvasMock, SetLineColor));
	fakeit::Fake(Method(canvasMock, SetFillColor));
	fakeit::Fake(Method(canvasMock, SetLineThickness));

	auto& canvas = canvasMock.get();

	GIVEN("a group of shapes")
	{
		GroupShape shapes;

		WHEN("group created")
		{
			THEN("it's empty")
			{
				CHECK(shapes.GetShapeCount() == 0);
			}
			AND_THEN("the frame is null")
			{
				CHECK_FALSE(shapes.GetFrame().has_value());
			}
			AND_THEN("can't access any elements")
			{
				CHECK_THROWS(shapes.GetShapeAtIndex(0));
				CHECK_THROWS(shapes.RemoveShapeAtIndex(0));
			}
			AND_THEN("fill style is empty")
			{
				const auto fillStyle = shapes.GetFillStyle();

				CHECK_FALSE(fillStyle->IsEnabled().has_value());
				CHECK_FALSE(fillStyle->GetColor().has_value());
			}
			AND_THEN("outline style is empty")
			{
				const auto outlineStyle = shapes.GetOutlineStyle();

				CHECK_FALSE(outlineStyle->IsEnabled().has_value());
				CHECK_FALSE(outlineStyle->GetThickness().has_value());
				CHECK_FALSE(outlineStyle->GetColor().has_value());
			}

			AND_WHEN("trying to draw it")
			{
				shapes.Draw(canvas);

				THEN("nothing is drawn")
				{
					fakeit::VerifyNoOtherInvocations(canvasMock);
				}
			}
		}

		WHEN("adding single shape")
		{
			auto shape1 = std::make_shared<CRectangle>(PointD{ 100, 100 }, 50, 30);
			shapes.InsertShape(shape1, 0);

			THEN("group has new shape")
			{
				CHECK(shapes.GetShapeCount() == 1);
			}
			AND_THEN("group frame == shape frame")
			{
				CHECK(shapes.GetFrame() == shape1->GetFrame());
			}
			AND_THEN("fill and outline properties match")
			{
				CHECK(shapes.GetFillStyle()->GetColor() == shape1->GetFillStyle()->GetColor());
				CHECK(shapes.GetOutlineStyle()->GetColor() == shape1->GetOutlineStyle()->GetColor());
				CHECK(shapes.GetOutlineStyle()->GetThickness() == shape1->GetOutlineStyle()->GetThickness());
			}

			AND_WHEN("changing group frame")
			{
				shapes.SetFrame({ 0, 0, 100, 60 });

				THEN("shape frame is moved and scaled accordingly")
				{
					CHECK(shape1->GetFrame() == RectD{ 0, 0, 100, 60 });
				}
			}
			AND_WHEN("trying to acces that shape")
			{
				THEN("no errors are thrown")
				{
					CHECK_NOTHROW(shapes.GetShapeAtIndex(0));
					CHECK_NOTHROW(shapes.RemoveShapeAtIndex(0));

					AND_THEN("there no shapes after removing")
					{
						CHECK(shapes.GetShapeCount() == 0);
					}
				}
			}
			AND_WHEN("drawing the frame")
			{
				canvasMock.ClearInvocationHistory();
				shapes.Draw(canvas);

				THEN("that single shape is drawn")
				{
					fakeit::Verify(Method(canvasMock, DrawPolygon)).Once();
				}
			}
			AND_WHEN("adding another shape")
			{
				auto shape2 = std::make_shared<CEllipse>(PointD{ 100, 100 }, 50, 30);
				shapes.InsertShape(shape2, 0);

				THEN("it placed before first shape")
				{
					CHECK(shapes.GetShapeCount() == 2);
					CHECK(shapes.GetShapeAtIndex(0)->GetFrame() == shape2->GetFrame());
				}
				AND_THEN("group frame is combined from shape frames")
				{
					CHECK(shapes.GetFrame() == RectD{ 50, 70, 100, 60 });
				}
				AND_THEN("styles of frame match")
				{
					CHECK(shapes.GetFillStyle()->GetColor() == shape2->GetFillStyle()->GetColor());
					CHECK(shapes.GetOutlineStyle()->GetColor() == shape2->GetOutlineStyle()->GetColor());
					CHECK(shapes.GetOutlineStyle()->GetThickness() == shape2->GetOutlineStyle()->GetThickness());
				}

				WHEN("changing fill color in one of the shapes")
				{
					shape1->GetFillStyle()->SetColor(0x0000FF);

					THEN("group fill color becomes null")
					{
						CHECK_FALSE(shapes.GetFillStyle()->GetColor().has_value());
					}
				}
				AND_WHEN("one of styles is disabled")
				{
					shape1->GetFillStyle()->Disable();
					THEN("one style becomes null")
					{
						CHECK_FALSE(shapes.GetFillStyle()->IsEnabled().has_value());
						CHECK(shapes.GetOutlineStyle()->IsEnabled().has_value());
					}
				}
				AND_WHEN("changing group frame")
				{
					shapes.SetFrame({ 0, 0, 100, 60 });

					THEN("shape frame is moved and scaled accordingly")
					{
						CHECK(shape2->GetFrame() == RectD{ 0, 0, 100, 60 });
					}
				}
				AND_WHEN("changing fill color for one of the shapes")
				{
					shape1->GetFillStyle()->SetColor(0xFF0000);

					THEN("groups color becomes null")
					{
						CHECK_FALSE(shapes.GetFillStyle()->GetColor().has_value());
					}
				}
				AND_WHEN("removing previous shape")
				{
					shapes.RemoveShapeAtIndex(1);

					THEN("only last shape is left")
					{
						CHECK(shapes.GetShapeCount() == 1);
						CHECK(shapes.GetShapeAtIndex(0)->GetFrame() == shape2->GetFrame());
					}
				}
				AND_WHEN("drawing the frame")
				{
					canvasMock.ClearInvocationHistory();
					shapes.Draw(canvas);

					THEN("both shapes are drawn")
					{
						fakeit::Verify(Method(canvasMock, DrawPolygon)).Once();
						fakeit::Verify(Method(canvasMock, DrawEllipse)).Once();
					}
				}
				AND_WHEN("changing group style")
				{
					shapes.GetOutlineStyle()->SetColor(0xff00ffff);
					shapes.GetOutlineStyle()->SetThickness(50);
					shapes.GetFillStyle()->SetColor(0xff00ffff);

					THEN("all shapes in group have new style")
					{
						for (size_t i = 0; i < shapes.GetShapeCount(); i++)
						{
							const auto fig = shapes.GetShapeAtIndex(i);
							CHECK(fig->GetFillStyle()->GetColor() == 0xff00ffff);
							CHECK(fig->GetOutlineStyle()->GetColor() == 0xff00ffff);
							CHECK(fig->GetOutlineStyle()->GetThickness() == 50);
						}
					}
				}
				AND_WHEN("adding an empty group at the end")
				{
					auto childGroup = std::make_shared<GroupShape>();
					shapes.InsertShape(childGroup, 1);

					THEN("frame is not null")
					{
						CHECK(shapes.GetFrame().has_value());
					}
				}
			}
		}
	}
}