#include "../RtRelation.h"
#include <gtest/gtest.h>

using namespace std;

//TODO implement Tests for RtRelation
class RtRelationTest : public ::testing::Test
{
public:
	RtRelationTest()
	{
		std::unique_ptr<std::array<double,800>> a(new std::array<double,800>);
		std::unique_ptr<std::array<double,800>> b(new std::array<double,800>);
		a->fill(0xFFFFFFFF);
		b->fill(0);
		rt1 = new RtRelation(move(a));
		rt2 = new RtRelation(move(b));

	}

	~RtRelationTest()
	{
		delete rt1;
		delete rt2;
	}

protected:
	RtRelation* rt1;
	RtRelation* rt2;

};

TEST_F(RtRelationTest,TestDummy)
{
	ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
