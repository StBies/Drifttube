#include "../Drifttube.h"
#include <gtest/gtest.h>

class DrifttubeTest : public ::testing::Test
{
public:
	DrifttubeTest()
	{
		d1.Drifttube(1,2,new DataSet());
		d2.Drifttube(3,4,new DataSet());
	}

	~DrifttubeTest()
	{

	}

protected:
	Drifttube d1;
	Drifttube d2;

};
