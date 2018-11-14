#include "../DriftTimeSpectrum.h"
#include "../Event.h"
#include <gtest/gtest.h>

using namespace std;

class DriftTimeSpectrumTest : public ::testing::Test
{
public:
	DriftTimeSpectrumTest()
	{
		unique_ptr<vector<uint32_t>> a(new vector<uint32_t>(800,0xFFFFFFFF));
		unique_ptr<vector<uint32_t>> b(new vector<uint32_t>(800,0));

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

TEST_F(DriftTimeSpectrumTest,TestCopyConstruction)
{
	DriftTimeSpectrum copyOfSpect1(*spect1);


	ASSERT_FALSE(&copyOfSpect1 == spect1);
	ASSERT_EQ(spect1->getData(), copyOfSpect1.getData());
	ASSERT_FALSE(&spect1->getData() == &copyOfSpect1.getData());
	ASSERT_EQ(spect1->getEntries(), copyOfSpect1.getEntries());
	ASSERT_EQ(spect1->getRejected(), copyOfSpect1.getRejected());
}

TEST_F(DriftTimeSpectrumTest,TestMaxValue)
{
	uint64_t maxInt = 0xFFFFFFFF;
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
	unique_ptr<vector<uint16_t>> a(new vector<uint16_t>(800));
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

TEST_F(DriftTimeSpectrumTest,TestAssignmentOperator)
{
	DriftTimeSpectrum a = *spect1;

	ASSERT_EQ(spect1->getEntries(),a.getEntries());
	ASSERT_EQ(spect1->getRejected(),a.getRejected());
	ASSERT_EQ(spect1->getData(),a.getData());
	ASSERT_FALSE(&spect1->getData() == &a.getData());

	DriftTimeSpectrum b = DriftTimeSpectrum(*spect1);

	ASSERT_EQ(spect1->getEntries(),b.getEntries());
	ASSERT_EQ(spect1->getRejected(),b.getRejected());
	ASSERT_EQ(spect1->getData(),b.getData());
	ASSERT_FALSE(&spect1->getData() == &b.getData());
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
