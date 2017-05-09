/*
 * DrifttubeTest.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: bieschke
 */

#ifdef GTEST

#include "gtest/gtest.h"
#include "../Drifttube.h"


class DrifttubeTest : public ::testing::Test {
public:
	DrifttubeTest()
	{
	}

	virtual void SetUp()
	{

	}

	virtual void TearDown()
	{

	}

};

TEST_F (DrifttubeTest,getRadius)
{
	Drifttube tube(0,0);
	ASSERT_EQ(18150,tube.getRadius());
}

TEST_F (DrifttubeTest,getPosition)
{
	Drifttube tube(1,-5);
	ASSERT_EQ(1,tube.getPositionX());
	ASSERT_EQ(-5,tube.getPositionY());
}

#else

#include "../Drifttube.h"

class DrifttubeTest : public CppUnit::TestCase {
public:
	DrifttubeTest()
	{
	}

	virtual void SetUp()
	{

	}

	virtual void TearDown()
	{

	}

};

#endif
