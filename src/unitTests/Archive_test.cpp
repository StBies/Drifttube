#include "../Archive.h"
#include <gtest/gtest.h>
#include <array>
#include <string>

using namespace std;

class ArchiveTest : public ::testing::Test
{
public:
	ArchiveTest()
	{
		a = new Archive("unitTestingData.drift");
	}

	~ArchiveTest()
	{
	}

protected:
	Archive* a;

};

TEST_F(ArchiveTest,Testtest)
{
	ASSERT_EQ(1,1);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
