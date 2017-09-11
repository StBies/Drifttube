/*
 * TriggerEventCollection_test.cpp
 *
 *  Created on: Aug 28, 2017
 *      Author: bieschke
 */

#include "../TriggerEventCollection.h"
#include <gtest/gtest.h>

class TriggerEventCollectionTest : public ::testing::Test
{
public:
	TriggerEventCollectionTest()
	{

	}

	void SetUp()
	{

	}

	void TearDown()
	{

	}

	~TriggerEventCollectionTest()
	{

	}

protected:
};

TEST_F(TriggerEventCollectionTest,Dummy)
{
	ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

