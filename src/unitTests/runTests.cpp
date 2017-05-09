/*
 * runTests.cpp
 *
 *  Created on: Apr 21, 2017
 *      Author: bieschke
 */

#include "gtest/gtest.h"

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


