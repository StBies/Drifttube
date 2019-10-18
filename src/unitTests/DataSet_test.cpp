#include "../Drifttube.h"
#include <gtest/gtest.h>

using namespace std;

uint32_t DataSetTestSize = 100000;

class DataSetTest : public ::testing::Test
{
public:
	DataSetTest()
	{
		d1 = new DataSet();
		vector<unique_ptr<Event>> initVector(DataSetTestSize);
		#pragma omp parallel for
		for(int i = 0; i < DataSetTestSize; i++)
		{
			unique_ptr<vector<uint16_t>> arr(new vector<uint16_t>(800,i));
			unique_ptr<Event> a(new Event(i,move(arr)));
			initVector[i] = move(a);
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
	ASSERT_EQ(DataSetTestSize,d2->getSize());

	for(size_t i = 0; i < DataSetTestSize; i++)
	{
		ASSERT_EQ((uint16_t)i,(*d2)[i][0]);
	}
}

TEST_F(DataSetTest,TestCopyConstruction)
{
	//test copy construction: copy d2
	DataSet copyOfD2(*d2);

	ASSERT_EQ(DataSetTestSize,copyOfD2.getSize());
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
	vector<uint16_t> test1(800,42), test2(800,1337);

	//create a unique pointer to an Event containing an array filled with 42 and push it to d1
	unique_ptr<vector<uint16_t>> arr(new vector<uint16_t>(800,42));
	unique_ptr<Event> evt(new Event(13378,move(arr)));
	d1->addData(move(evt));

	//remember old size
	size_t oldSize = d2->getSize();

	//create a unique pointer to an Event containing an array filled with 1337 and push it to d2
	unique_ptr<vector<uint16_t>> arr2(new vector<uint16_t>(800,1337));
	unique_ptr<Event> evt2(new Event(13378,move(arr2)));
	d2->addData(move(evt2));


	ASSERT_EQ(1,d1->getSize());
	ASSERT_EQ(oldSize + 1, d2->getSize());
	ASSERT_TRUE(test1 == (*d1)[d1->getSize()-1].getData());
	ASSERT_TRUE(test2 == (*d2)[d2->getSize()-1].getData());
}

TEST_F(DataSetTest,TestMeanOffsetVoltage)
{
	//TEST for empty DataSet
	ASSERT_EQ(0,d1->get_mean_offset_voltage());
	//TEST for filled dataset
	vector<unique_ptr<Event>> initVector(2);
	#pragma omp parallel for
	for(int i = 0; i < 2; i++)
	{
		unique_ptr<vector<uint16_t>> arr(new vector<uint16_t>(800,i+1));
		unique_ptr<Event> a(new Event(i,move(arr)));
		initVector[i] = move(a);
	}
	initVector.shrink_to_fit();
	DataSet* d = new DataSet(initVector);
	//d contains two events, one all filled with 1, and one all filled with 2
	//this should result in a mean offset voltage of 1.5
	ASSERT_DOUBLE_EQ(1.5,d->get_mean_offset_voltage());
	delete d;

	//TEST for build with empty vector which should result in -1 set but a valid construction run.
	initVector.resize(0);

	d = new DataSet(initVector);
	ASSERT_DOUBLE_EQ(-1,d->get_mean_offset_voltage());
	delete d;
}

TEST_F(DataSetTest,Test_mean_noise_amplitude)
{
	//Test for empty:
	ASSERT_EQ(0,d1->get_mean_offset_voltage());

	//Test for filled DataSet
	vector<unique_ptr<Event>> initVector(2);
	#pragma omp parallel for
	for(int i = 0; i < 2; i++)
	{
		unique_ptr<vector<uint16_t>> arr(new vector<uint16_t>(800,i+1));
		unique_ptr<Event> a(new Event(i,move(arr)));
		initVector[i] = move(a);
	}
	initVector.shrink_to_fit();
	DataSet* d = new DataSet(initVector);
	//expect sqrt[1 / 2 ( (1-1.5)^2 + (2 - 1.5)^2)] = sqrt(1/4) = 0.5
	ASSERT_DOUBLE_EQ(0.5,d->get_mean_noise_amplitude());
	delete d;

	//Test for invalid built DataSet
	//TEST for build with empty vector which should result in -1 set but a valid construction run.
	initVector.resize(0);

	d = new DataSet(initVector);
	ASSERT_DOUBLE_EQ(-1,d->get_mean_noise_amplitude());
	delete d;
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
