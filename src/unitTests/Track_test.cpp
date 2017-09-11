/*
 * Track_test.cpp
 *
 *  Created on: Aug 29, 2017
 *      Author: bieschke
 */

#include "../Track.h"
#include <gtest/gtest.h>

class TrackTest : public ::testing::Test
{
public:
	TrackTest()
	{

	}

	void SetUp()
	{

	}

	void TearDown()
	{

	}

	~TrackTest()
	{

	}

protected:
};

TEST_F(TrackTest,Dummy)
{
	ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

