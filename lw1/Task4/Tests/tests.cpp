#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../ShapesProgram/Shape/Shape.h"
#include "../ShapesProgram/Shape/ShapeStrategy/IShapeStrategy.h"
#include "../ShapesProgram/gfx/ICanvas.h"
#include "../ShapesProgram/Shape/Picture.h"

using namespace testing;
using namespace shapes;
using namespace gfx;

class MockCanvas : public ICanvas 
{
public:
    MOCK_METHOD(void, MoveTo, (double x, double y), (override));
    MOCK_METHOD(void, SetColor, (const Color& color), (override));
    MOCK_METHOD(void, LineTo, (double dx, double dy), (override));
    MOCK_METHOD(void, DrawRectangle, (const Point& start, double width, double height), (override));
    MOCK_METHOD(void, DrawTriangle, (const Point& p1, const Point& p2, const Point& p3), (override));
    MOCK_METHOD(void, DrawEllipse, (double cx, double cy, double rx, double ry), (override));
    MOCK_METHOD(void, DrawText, (double left, double top, double fontSize, const std::string& text), (override));
};

class MockShapeStrategy : public IShapeStrategy 
{
public:
    MOCK_METHOD(void, Draw, (ICanvas* canvas, Color color), (const, override));
    MOCK_METHOD(std::string, GetInfo, (), (const, override));
    MOCK_METHOD(std::string, GetType, (), (const, override));
    MOCK_METHOD(void, Move, (double dx, double dy), (override));
};

class ShapeTest : public Test 
{
protected:
    void SetUp() override 
    {
        mockStrategy = std::make_unique<MockShapeStrategy>();
        strategyPtr = mockStrategy.get();
        shape = std::make_unique<Shape>(std::move(mockStrategy));
    }

    std::unique_ptr<MockShapeStrategy> mockStrategy;
    MockShapeStrategy* strategyPtr = nullptr;
    std::unique_ptr<Shape> shape;
    MockCanvas canvas;
};

TEST_F(ShapeTest, Draw_ShouldCallStrategyDrawWithColor) 
{
    Color expectedColor("#FF0000");
    shape->SetColor(expectedColor);

    EXPECT_CALL(*strategyPtr, Draw(&canvas, expectedColor));

    shape->Draw(&canvas);
}

TEST_F(ShapeTest, GetInfo_ShouldReturnStrategyInfo) 
{
    std::string expectedInfo = "Test Info";
    EXPECT_CALL(*strategyPtr, GetInfo()).WillOnce(Return(expectedInfo));

    ASSERT_EQ(shape->GetInfo(), expectedInfo);
}

TEST_F(ShapeTest, Move_ShouldDelegateToStrategy) 
{
    EXPECT_CALL(*strategyPtr, Move(10.5, -3.2));

    shape->Move(10.5, -3.2);
}

TEST_F(ShapeTest, SetColor_ShouldChangeColor) 
{
    Color newColor("#00FF00");
    shape->SetColor(newColor);

    ASSERT_EQ(shape->GetColor().m_data, newColor.m_data);
}

TEST_F(ShapeTest, SetStrategy_ShouldReplaceStrategy) 
{
    auto newStrategy = std::make_unique<MockShapeStrategy>();
    auto* newStrategyPtr = newStrategy.get();

    EXPECT_CALL(*newStrategyPtr, GetType()).WillOnce(Return("NewType"));

    shape->SetStrategy(std::move(newStrategy));

    ASSERT_EQ(shape->GetType(), "NewType");
}

class PictureTest : public Test 
{
protected:
    void SetUp() override 
    {
        picture = std::make_unique<Picture>();
        mockStrategy = std::make_unique<MockShapeStrategy>();
        strategyPtr = mockStrategy.get();
    }

    std::unique_ptr<Picture> picture;
    std::unique_ptr<MockShapeStrategy> mockStrategy;
    MockShapeStrategy* strategyPtr;
    MockCanvas canvas;
};

TEST_F(PictureTest, AddShape_ShouldStoreShape)
{
    auto shape = std::make_unique<Shape>(std::move(mockStrategy));
    auto* shapePtr = shape.get();

    picture->AddShape("test", std::move(shape));

    ASSERT_EQ(picture->GetShape("test"), shapePtr);
}

TEST_F(PictureTest, GetMissingShape_ShouldReturnNullptr) 
{
    ASSERT_EQ(picture->GetShape("missing"), nullptr);
}

TEST_F(PictureTest, DeleteShape_ShouldRemoveShape) 
{
    auto shape = std::make_unique<Shape>(std::move(mockStrategy));
    picture->AddShape("test", std::move(shape));

    picture->DeleteShape("test");

    ASSERT_EQ(picture->GetShape("test"), nullptr);
}

TEST_F(PictureTest, MovePicture_ShouldMoveAllShapes) 
{
    auto shape1 = std::make_unique<Shape>(std::make_unique<MockShapeStrategy>());
    auto shape2 = std::make_unique<Shape>(std::make_unique<MockShapeStrategy>());

    EXPECT_CALL(*strategyPtr, Move(2.5, 3.5)).Times(2);

    picture->AddShape("s1", std::move(shape1));
    picture->AddShape("s2", std::move(shape2));
    picture->MovePicture(2.5, 3.5);
}

TEST_F(PictureTest, DrawPicture_ShouldDrawAllShapes) 
{
    auto shape1 = std::make_unique<Shape>(std::make_unique<MockShapeStrategy>());
    auto shape2 = std::make_unique<Shape>(std::make_unique<MockShapeStrategy>());

    EXPECT_CALL(*strategyPtr, Draw(&canvas, _)).Times(2);

    picture->AddShape("s1", std::move(shape1));
    picture->AddShape("s2", std::move(shape2));
    picture->DrawPicture(&canvas);
}

TEST_F(PictureTest, GetAllInfo_ShouldReturnCorrectInfo) 
{
    auto shape = std::make_unique<Shape>(std::move(mockStrategy));
    EXPECT_CALL(*strategyPtr, GetInfo()).WillOnce(Return("Circle info"));

    picture->AddShape("circle", std::move(shape));

    ASSERT_THAT(picture->GetAllInfo(), HasSubstr("circle: Circle info"));
}