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

TEST_F(ArchiveTest,Testtest)
{
	cout << "efficiency: " << a->getTubes()[0]->getEfficiency() << endl;
	ASSERT_EQ(1,1);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
