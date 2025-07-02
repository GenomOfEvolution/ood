#include "pch.h"
#include "../DuckDance/lib/Duck/ModelDuck.h"
#include "../DuckDance/lib/Fly/FlyNoWay.h"
#include "../DuckDance/lib/Quack/MuteQuackBehavior.h"
#include "../DuckDance/lib/Dance/MockDance.h"
#include "../DuckDance/lib/DuckFunctions.h"

TEST(DuckBehavior, MockDuckDance) 
{
	auto mockBehavior = std::make_unique<MockDance>();
	auto* mockPtr = mockBehavior.get();

	ModelDuck duck;
	duck.SetDanceBehavior(std::move(mockBehavior));
	
	EXPECT_FALSE(mockPtr->GetDanceStatus());
	duck.Dance();
	EXPECT_TRUE(mockPtr->GetDanceStatus());
}