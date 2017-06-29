#include "../DataProcessor.h"
#include <gtest/gtest.h>
#include <array>
#include "../Event.h"
#include "../RtRelation.h"
#include "../DriftTimeSpectrum.h"

using namespace std;

class DataProcessorTest : public ::testing::Test
{
public:
	DataProcessorTest()
	{
		unique_ptr<array<uint16_t,800>> sine_array(new array<uint16_t,800>);
		unique_ptr<array<uint16_t,800>> const1_array(new array<uint16_t,800>);
		unique_ptr<array<uint16_t,800>> const0_array(new array<uint16_t,800>);
		unique_ptr<array<uint16_t,800>> max_uint_array(new array<uint16_t,800>);
		unique_ptr<array<uint16_t,800>> min_at_400_array(new array<uint16_t,800>);

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
	array<int,800> const1_int_exp;
	array<int,800> const0_int_exp;
	array<int,800> max_uint_int_exp;

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
	ASSERT_EQ(400,DataProcessor::findDriftTime(*min_at_400,6));
	ASSERT_EQ(-42,DataProcessor::findDriftTime(*max_uint,100));
}

TEST_F(DataProcessorTest,TestFindLastFilledBin)
{
	ASSERT_EQ(400,DataProcessor::findLastFilledBin(*min_at_400,6));
	ASSERT_EQ(799,DataProcessor::findLastFilledBin(*const1,1));
	ASSERT_EQ(0,DataProcessor::findLastFilledBin(*const1,0));
}

TEST_F(DataProcessorTest,TestcalculateRtRelation)
{
	unique_ptr<array<uint32_t,800>> dtArr(new array<uint32_t,800>);
	dtArr->fill(0);
	(*dtArr)[30] = 1;
	DriftTimeSpectrum spect(move(dtArr),1,0);
	RtRelation rt = DataProcessor::calculateRtRelation(spect);

	array<double,800> expectedRt;
	expectedRt.fill(0);
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

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
