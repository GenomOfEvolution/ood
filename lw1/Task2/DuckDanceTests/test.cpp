#include "pch.h"
#include "../DuckDance/lib/Duck/ModelDuck.h"
#include "../DuckDance/lib/Fly/MockQuackOnDoubleFly.h"
#include "../DuckDance/lib/Quack/MockQuackBehavior.h"
#include "../DuckDance/lib/DuckFunctions.h"

TEST(DuckDanceMultipleTimes, QuackOnDoubleFly) 
{
	auto mockQuack = std::make_unique<MockQuackBehavior>();
	auto* mockQuackPtr = mockQuack.get();

	auto mockFly = std::make_unique<MockQuackOnDoubleFly>();
	auto* mockFlyPtr = mockFly.get();

	EXPECT_FALSE(mockQuackPtr->WasQuack());
	EXPECT_EQ(mockFlyPtr->GetFlightCount(), 0);

	ModelDuck duck;
	duck.SetFlyBehavior(std::move(mockFly));
	duck.SetQuackBehavior(std::move(mockQuack));

	duck.Fly();
	EXPECT_EQ(mockFlyPtr->GetFlightCount(), 1);
	EXPECT_FALSE(mockQuackPtr->WasQuack());

	duck.Fly();
	EXPECT_EQ(mockFlyPtr->GetFlightCount(), 2);
	EXPECT_TRUE(mockQuackPtr->WasQuack());
}