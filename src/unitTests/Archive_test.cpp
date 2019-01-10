#include "../Archive.h"
#include <gtest/gtest.h>
#include <array>
#include <string>
#include <iostream>

using namespace std;

class ArchiveTest : public ::testing::Test
{
public:
	ArchiveTest()
	{
		a = new Archive("data/unitTestingData.drift");
	}

	~ArchiveTest()
	{
	}

protected:
	Archive* a;
};

TEST_F(ArchiveTest,TestBinaryRead)
{
	uint32_t nTubes = a->getTubes().size();
	ASSERT_EQ(1,nTubes);
	uint32_t sizeOfTubeDataSet = a->getTubes()[0]->getDataSet().getSize();
	ASSERT_EQ(17069,sizeOfTubeDataSet);
}

TEST_F(ArchiveTest,TestParser)
{
	ASSERT_EQ(0,a->getDirname().compare("data/"));
	ASSERT_EQ(0,a->getFilename().compare("unitTestingData.drift"));
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
