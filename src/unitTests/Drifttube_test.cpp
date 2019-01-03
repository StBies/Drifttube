#include "../Drifttube.h"
#include <gtest/gtest.h>
#include "../globals.h"

using namespace std;

class DrifttubeTest : public ::testing::Test
{
public:
	DrifttubeTest()
	{
		unique_ptr<DataSet> d1_data(new DataSet());
		d1 = new Drifttube(1,2,move(d1_data));

		unique_ptr<DataSet> d2_data(new DataSet());
		d2 = new Drifttube(3,4,move(d2_data));

		//create data for a DataSet containing one event with a voltage under 2200-50 channels in bin 50 (real dt = 40ns)
		vector<unique_ptr<Event>> data;
		unique_ptr<vector<uint16_t>> arr(new vector<uint16_t>(800,OFFSET_ZERO_VOLTAGE));
		(*arr)[50] = OFFSET_ZERO_VOLTAGE + (2 * EVENT_THRESHOLD_VOLTAGE);
		unique_ptr<Event> e1(new Event(1,move(arr)));
		data.push_back(move(e1));
		data.shrink_to_fit();
		d3_filled = new Drifttube(5,6,move(unique_ptr<DataSet>(new DataSet(data))));
	}

	~DrifttubeTest()
	{
		delete d1;
		delete d2;
		delete d3_filled;
	}

protected:
	Drifttube* d1;
	Drifttube* d2;
	Drifttube* d3_filled;
};

TEST_F(DrifttubeTest,TestCopyConstruction)
{
	Drifttube copyOfD3(*d3_filled);
	ASSERT_FALSE(d3_filled == &copyOfD3);


	//Test, if the position holds the same values at another memory location
	ASSERT_EQ(d3_filled->getPosition(), copyOfD3.getPosition());
	ASSERT_FALSE(&d3_filled->getPosition() == &copyOfD3.getPosition());

	//Test, if the DriftTimeSpectrum holds the same values at another memory location
	ASSERT_EQ(d3_filled->getDriftTimeSpectrum().getData(), copyOfD3.getDriftTimeSpectrum().getData());
	ASSERT_FALSE(&d3_filled->getDriftTimeSpectrum() == &copyOfD3.getDriftTimeSpectrum());
	ASSERT_EQ(d3_filled->getDriftTimeSpectrum().getEntries(), copyOfD3.getDriftTimeSpectrum().getEntries());
	ASSERT_EQ(d3_filled->getDriftTimeSpectrum().getRejected(), copyOfD3.getDriftTimeSpectrum().getRejected());

	//Test, if the RtRelation holds the same values at another memory location
	//FIXME this doesn't pass
	ASSERT_EQ(d3_filled->getRtRelation().getData(), copyOfD3.getRtRelation().getData());
	ASSERT_FALSE(&d3_filled->getRtRelation() == &copyOfD3.getRtRelation());
}

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

TEST_F(DrifttubeTest,TestDriftTimeSpectrum)
{
	unique_ptr<vector<uint32_t>> dt(new vector<uint32_t>(800,0));
	(*dt)[50] = 1;
	DriftTimeSpectrum wanted(move(dt),1,0);
	ASSERT_EQ(wanted.getEntries(),d3_filled->getDriftTimeSpectrum().getEntries());
	ASSERT_EQ(wanted.getRejected(),d3_filled->getDriftTimeSpectrum().getRejected());
	ASSERT_EQ(wanted.getData(),(*d3_filled).getDriftTimeSpectrum().getData());
}

TEST_F(DrifttubeTest,TestRtRelation)
{
	unique_ptr<vector<double>> rt(new vector<double>(800,18.15));
	for(size_t i = 0; i < 51; i++)
	{
		(*rt)[i] = 0;
	}
	RtRelation wanted(move(rt));

	for(size_t i = 0; i < wanted.getData().size(); i++)
	{
		ASSERT_DOUBLE_EQ(wanted[i],d3_filled->getRtRelation()[i]);
	}
}

TEST_F(DrifttubeTest,TestEfficiency)
{
	//TODO more testcases
	ASSERT_EQ(1,d3_filled->getEfficiency());
}

TEST_F(DrifttubeTest,TestAssignmentOperators)
{
	unique_ptr<DataSet> data(new DataSet(d3_filled->getDataSet()));
	Drifttube d(*d1);
	//Test const assignment operator
	d = Drifttube(0,0,move(data));
	//Test non const assignment operator
	Drifttube e = *d2;
	e = d;

//	ASSERT_FALSE(&a[0].getDataSet() == &d.getDataSet());

	ASSERT_EQ(nullptr,data.get());
	ASSERT_FALSE(&d.getDataSet() == data.get());
	ASSERT_FALSE(&e == &d);
	for(size_t i = 0; i < d.getDataSet().getSize(); i++)
	{
		ASSERT_EQ(d3_filled->getDataSet()[i].getData(),d.getDataSet()[i].getData());
		ASSERT_EQ(e.getDataSet()[i].getData(),d.getDataSet()[i].getData());
	}
}

TEST_F(DrifttubeTest,TestMaxDrifttime)
{
	ASSERT_EQ((50 - ADC_TRIGGERPOS_BIN + 1) * 4,d3_filled->getMaxDrifttime());
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
