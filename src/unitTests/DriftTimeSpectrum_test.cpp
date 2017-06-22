#include "../DriftTimeSpectrum.h"
#include "../Event.h"
#include <gtest/gtest.h>

using namespace std;

class DriftTimeSpectrumTest : public ::testing::Test
{
public:
	DriftTimeSpectrumTest()
	{
		std::unique_ptr<std::array<uint32_t,800>> a(new std::array<uint32_t,800>);
		std::unique_ptr<std::array<uint32_t,800>> b(new std::array<uint32_t,800>);
		a->fill(0xFFFFFFFF);
		b->fill(0);
		spect1 = new DriftTimeSpectrum(move(a),25000,0);
		spect2 = new DriftTimeSpectrum(move(b),0,0);

	}

	~DriftTimeSpectrumTest()
	{
		delete spect1;
		delete spect2;
	}

protected:
	DriftTimeSpectrum* spect1;
	DriftTimeSpectrum* spect2;

};

TEST_F(DriftTimeSpectrumTest,TestMaxValue)
{
	unsigned long maxInt = 0xFFFFFFFF;
	ASSERT_TRUE(maxInt + 1 > maxInt);
	for(unsigned short i = 0; i < spect1->getData().size(); i++)
	{
		ASSERT_EQ(maxInt,(*spect1)[i]);
	}
}

TEST_F(DriftTimeSpectrumTest,TestGetEntries)
{
	ASSERT_EQ(25000,spect1->getEntries());
	ASSERT_EQ(0,spect2->getEntries());
}

TEST_F(DriftTimeSpectrumTest,TestCapacityVsEvent)
{
	unique_ptr<array<uint16_t,800>> a(new array<uint16_t,800>);
	Event e(1,move(a));
	e[0] = 0xFFFF;
	e[0]++;

	(*spect1)[0] = 0xFFFF + 1;

	ASSERT_TRUE(e[0] < (*spect1)[0]);
	ASSERT_EQ(0,e[0]);
	ASSERT_EQ((unsigned long)0x10000,(*spect1)[0]);
}

TEST_F(DriftTimeSpectrumTest,TestOverflowAtZero)
{
	(*spect1)[0]++;
	ASSERT_EQ(0,(*spect1)[0]);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
