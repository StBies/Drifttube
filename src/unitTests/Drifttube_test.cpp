#include "../Drifttube.h"
#include <gtest/gtest.h>

using namespace std;

class DrifttubeTest : public ::testing::Test
{
public:
	DrifttubeTest()
	{
		d1 = new Drifttube(1,2,move(unique_ptr<DataSet>(new DataSet())));
		d2 = new Drifttube(3,4,move(unique_ptr<DataSet>(new DataSet())));
	}

	~DrifttubeTest()
	{
		delete d1;
		delete d2;
	}

protected:
	Drifttube* d1;
	Drifttube* d2;

};

TEST_F(DrifttubeTest,TestRadius)
{
	ASSERT_EQ(18150,d1->getRadius());
	ASSERT_EQ(18150,d2->getRadius());
	ASSERT_TRUE(d1->getRadius() == d2->getRadius());
}

TEST_F(DrifttubeTest,TestCoords)
{
	ASSERT_EQ(1,d1->getPositionX());
	ASSERT_EQ(2,d1->getPositionY());
	ASSERT_EQ(3,d2->getPositionX());
	ASSERT_EQ(4,d2->getPositionY());
}

TEST_F(DrifttubeTest,TestPosArray)
{
	//Test array coords
	ASSERT_EQ(1,d1->getPosition()[0]);
	ASSERT_EQ(2,d1->getPosition()[1]);
	ASSERT_EQ(3,d2->getPosition()[0]);
	ASSERT_EQ(4,d2->getPosition()[1]);

	//create new test-arrays to check the == operator for the array from the getter method
	array<int,2> test1 = {1,2};
	array<int,2> test2 = {3,4};
	ASSERT_TRUE(test1 == d1->getPosition());
	ASSERT_TRUE(test2 == d2->getPosition());

	ASSERT_EQ(2,d1->getPosition().size());
	ASSERT_EQ(2,d2->getPosition().size());
}

TEST_F(DrifttubeTest,TestDataSet)
{
	ASSERT_EQ(0,d1->getDataSet().getSize());
	ASSERT_EQ(0,d2->getDataSet().getSize());
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
