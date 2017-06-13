/*
 * ArgsParser_test.cpp
 *
 *  Created on: 07.06.2017
 *      Author: stefan
 */

#include "../ArgsParser.h"
#include <gtest/gtest.h>


using namespace std;

class ArgsParserTest : public ::testing::Test
{
public:
	ArgsParserTest()
	{

	}

	~ArgsParserTest()
	{
	}

protected:

};

TEST_F(ArgsParserTest,TestBla)
{
	ASSERT_TRUE(true);

}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
