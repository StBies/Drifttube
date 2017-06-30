#include "../RtRelation.h"
#include <gtest/gtest.h>
#include <cmath>

using namespace std;

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

TEST_F(RtRelationTest,TestCopyConstruction)
{
	RtRelation copy(*rt1);
	ASSERT_FALSE(rt1 == &copy);
	ASSERT_EQ(rt1->getData(), copy.getData());
	ASSERT_FALSE(&rt1->getData() == &copy.getData());
}

TEST_F(RtRelationTest,TestDoubleContainment)
{
	(*rt1)[0] = M_PI;
	ASSERT_DOUBLE_EQ(3.14159265358979323846, (*rt1)[0]);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
