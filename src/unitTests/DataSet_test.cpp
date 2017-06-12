#include "../Drifttube.h"
#include <gtest/gtest.h>

using namespace std;

class DataSetTest : public ::testing::Test
{
public:
	DataSetTest()
	{
		d1 = new DataSet();
		vector<unique_ptr<Event>> initVector;
		for(int i = 0; i < 15; i++)
		{
			unique_ptr<array<uint16_t,800>> arr(new array<uint16_t,800>);
			arr->fill(i);
			unique_ptr<Event> a(new Event(i,move(arr)));
			initVector.push_back(move(a));
		}
		initVector.shrink_to_fit();
		d2 = new DataSet(initVector);
	}

	~DataSetTest()
	{
		delete d1;
		delete d2;
	}

protected:
	DataSet* d1;
	DataSet* d2;

};

TEST_F(DataSetTest,TestConstruction)
{
	ASSERT_EQ(0,d1->getSize());
	ASSERT_EQ(15,d2->getSize());
	ASSERT_EQ(0,(*d2)[0][0]);
	ASSERT_EQ(1,(*d2)[1][0]);
	ASSERT_EQ(2,(*d2)[2][0]);

}

TEST_F(DataSetTest,TestCopyConstruction)
{
	//test copy construction: copy d2
	DataSet copyOfD2(*d2);

	ASSERT_EQ(15,copyOfD2.getSize());
	ASSERT_FALSE(d2 == &copyOfD2);
	ASSERT_TRUE(d2->getSize() == copyOfD2.getSize());


	for(unsigned int i = 0; i < d2->getSize(); i++)
	{
		ASSERT_FALSE(&(*d2)[i] == &copyOfD2[i]);
		ASSERT_FALSE(&copyOfD2[i].getData() == &(*d2)[i].getData());
		ASSERT_TRUE(copyOfD2[i].getData() == (*d2)[i].getData());
	}

}

TEST_F(DataSetTest,TestAddData)
{
	//build test arrays that are filled with 42 or 1337 respectively
	array<uint16_t,800> test1, test2;
	test1.fill(42);
	test2.fill(1337);

	//create a unique pointer to an Event containing an array filled with 42 and push it to d1
	unique_ptr<array<uint16_t,800>> arr(new array<uint16_t,800>);
	arr->fill(42);
	unique_ptr<Event> evt(new Event(13378,move(arr)));
	d1->addData(move(evt));

	//remember old size
	size_t oldSize = d2->getSize();

	//create a unique pointer to an Event containing an array filled with 1337 and push it to d2
	unique_ptr<array<uint16_t,800>> arr2(new array<uint16_t,800>);
	arr2->fill(1337);
	unique_ptr<Event> evt2(new Event(13378,move(arr2)));
	d2->addData(move(evt2));


	ASSERT_EQ(1,d1->getSize());
	ASSERT_EQ(oldSize + 1, d2->getSize());
	ASSERT_TRUE(test1 == (*d1)[d1->getSize()-1].getData());
	ASSERT_TRUE(test2 == (*d2)[d2->getSize()-1].getData());
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
