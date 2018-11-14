#include "../RtRelation.h"
#include <gtest/gtest.h>
#include <cmath>

using namespace std;

class RtRelationTest : public ::testing::Test
{
public:
	RtRelationTest()
	{
		unique_ptr<vector<double>> a(new vector<double>(800,0xFFFFFFFF));
		unique_ptr<vector<double>> b(new vector<double>(800,0));
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

TEST_F(RtRelationTest,TestAssignmentOperator)
{
	//first make a copy of rt1
	RtRelation r(*rt1);
	//assign rt2 to r - thus rt1 should not be stored in r anymore
	r = *rt2;
	ASSERT_FALSE(&r.getData() == &rt2->getData());
	ASSERT_FALSE(&r == rt2);
	for(size_t i = 0; i < r.getData().size(); i++)
	{
		ASSERT_EQ(rt2->getData()[i],r.getData()[i]);
	}
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
