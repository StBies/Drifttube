#include "../DataProcessor.h"
#include <gtest/gtest.h>
#include <array>
#include "../Event.h"
#include "../RtRelation.h"
#include "../DriftTimeSpectrum.h"
#include "../DataSet.h"

using namespace std;

class DataProcessorTest : public ::testing::Test
{
public:
	DataProcessorTest()
	{
		unique_ptr<vector<uint16_t>> sine_array(new vector<uint16_t>(800));
		unique_ptr<vector<uint16_t>> const1_array(new vector<uint16_t>(800));
		unique_ptr<vector<uint16_t>> const0_array(new vector<uint16_t>(800));
		unique_ptr<vector<uint16_t>> max_uint_array(new vector<uint16_t>(800));
		unique_ptr<vector<uint16_t>> min_at_400_array(new vector<uint16_t>(800));

		double sine_range_to_bins = M_PI / 800;

		for(int i = 0; i < 800; i++)
		{
			(*sine_array)[i] = (uint16_t)(sin(i * sine_range_to_bins) * 0xFFFF);
			(*const1_array)[i] = 1;
			(*const0_array)[i] = 0;
			(*max_uint_array)[i] = 0xFFFF;
			(*min_at_400_array)[i] = 10;
		}
		(*min_at_400_array)[400] = 5;

		sine = new Event(0,move(sine_array));
		const1 = new Event(1,move(const1_array));
		const0 = new Event(2,move(const0_array));
		max_uint = new Event(3,move(max_uint_array));
		min_at_400 = new Event(4,move(min_at_400_array));

	}

	~DataProcessorTest()
	{
	}

protected:
	Event* sine;
	Event* const1;
	Event* const0;
	Event* max_uint;
	Event* min_at_400;
};

TEST_F(DataProcessorTest,TestComputeIntegral)
{
	ASSERT_EQ(0,DataProcessor::computeIntegral(*const0));
	ASSERT_EQ(800,DataProcessor::computeIntegral(*const1));

	uint64_t expected_sine_halfwave_integral = (uint64_t)(0xFFFF * 2 * 800 / M_PI);
	int actualValue = DataProcessor::computeIntegral(*sine);
	bool not_smaller = actualValue >= expected_sine_halfwave_integral - 0.00005 * expected_sine_halfwave_integral;
	bool not_larger = actualValue <= expected_sine_halfwave_integral + 0.00005 * expected_sine_halfwave_integral;

	ASSERT_TRUE(not_smaller && not_larger);

}

TEST_F(DataProcessorTest,TestComputeIntegralOverflow)
{
	unsigned long result = 800 * 0xFFFF;
	ASSERT_EQ(result, DataProcessor::computeIntegral(*max_uint));
}

TEST_F(DataProcessorTest,TestIntegrate)
{
	vector<int> const1_int_exp(800);
	vector<int> const0_int_exp(800);
	vector<int> max_uint_int_exp(800);

	for(int i = 0; i < 800; i++)
	{
		const1_int_exp[i] = i;
		const0_int_exp[i] = 0;
		max_uint_int_exp[i] = i * 0xFFFF;
	}

	ASSERT_EQ(const1_int_exp,DataProcessor::integrate(*const1));
	ASSERT_EQ(const0_int_exp,DataProcessor::integrate(*const0));
	ASSERT_EQ(max_uint_int_exp,DataProcessor::integrate(*max_uint));
}

TEST_F(DataProcessorTest,TestFindMinumumBin)
{

	ASSERT_EQ(0,DataProcessor::findMinimumBin(*const0));
	ASSERT_EQ(0,DataProcessor::findMinimumBin(*const1));
	ASSERT_EQ(400,DataProcessor::findMinimumBin(*min_at_400));
}

TEST_F(DataProcessorTest,TestFindDriftTime)
{
	ASSERT_EQ(400,DataProcessor::findDriftTimeBin(*min_at_400,6));
	ASSERT_EQ(-42,DataProcessor::findDriftTimeBin(*max_uint,100));
}

TEST_F(DataProcessorTest,TestFindLastFilledBin)
{
	ASSERT_EQ(400,DataProcessor::findLastFilledBin(*min_at_400,6));
	ASSERT_EQ(799,DataProcessor::findLastFilledBin(*const1,1));
	ASSERT_EQ(0,DataProcessor::findLastFilledBin(*const1,0));
}

TEST_F(DataProcessorTest,TestcalculateRtRelation)
{
	unique_ptr<vector<uint32_t>> dtArr(new vector<uint32_t>(800,0));
	(*dtArr)[30] = 1;
	DriftTimeSpectrum spect(move(dtArr),1,0);
	RtRelation rt = DataProcessor::calculateRtRelation(spect);

	vector<double> expectedRt(800,0.0);
	for(size_t i = 31; i < expectedRt.size(); i++)
	{
		expectedRt[i] = DRIFT_TUBE_RADIUS;
	}
	//actual test
	for(size_t i = 0; i < expectedRt.size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedRt[i],rt.getData()[i]);
	}
}

TEST_F(DataProcessorTest,TestCalculateDriftTimeSpectrum)
{
	//TODO implement

	//Create an Event
	unique_ptr<vector<uint16_t>> data(new vector<uint16_t>(800,2200));
	data[50] = 2100;
	Event e(move(data));
	DataSet test_set(move(e));

	DriftTimeSpectrum spect = DataProcessor::calculateDriftTimeSpectrum(test_set);

	//Create a DataSet
	ASSERT_TRUE(false);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
