#include "../Event.h"
#include <gtest/gtest.h>

/**
 * Unit test for the Event class utilizing the Google Test Framework (see https://github.com/google/googletest )
 */
class EventTest : public ::testing::Test
{
public:
	EventTest()
	{
		std::unique_ptr<std::vector<uint16_t>> a(new std::vector<uint16_t>(800,42));
		std::unique_ptr<std::vector<uint16_t>> b(new std::vector<uint16_t>(800,1337));
		std::unique_ptr<std::vector<uint16_t>> c(new std::vector<uint16_t>(1024,1));

		e1 = new Event(42,move(a));
		e2 = new Event(1337,move(b));
		e3 = new Event(9001, move(c));
	}

	void SetUp()
	{

	}

	void TearDown()
	{

	}

	~EventTest()
	{
		delete e1;
		delete e2;
		delete e3;
	}

protected:
	Event* e1;
	Event* e2;
	Event* e3;
};

TEST_F(EventTest, TestEventNumber)
{
	ASSERT_EQ(42,e1->getEventNumber());
	ASSERT_EQ(1337,e2->getEventNumber());
}

TEST_F(EventTest, TestEventEntries)
{
	ASSERT_EQ(42,e1->getData().at(0));
	ASSERT_EQ(1337,e2->getData().at(5));
	ASSERT_EQ(1,e3->getData().at(42));
}

TEST_F(EventTest, TestEventSize)
{
	ASSERT_EQ(800,e1->getData().size());
	ASSERT_EQ(800,e2->getData().size());
	ASSERT_EQ(1024,e3->getData().size());
}

TEST_F(EventTest, TestGetSize)
{
	ASSERT_EQ(e1->getData().size(),e1->getSize());
	ASSERT_EQ(e2->getData().size(),e2->getSize());
	ASSERT_EQ(e3->getData().size(),e3->getSize());
}

TEST_F(EventTest, TestAccessOperator)
{
	ASSERT_TRUE(e1->getData()[42] == (*e1)[42]);
}

TEST_F(EventTest, TestAccessLvalue)
{
	(*e1)[42] = 43;
	ASSERT_EQ(43, (*e1)[42]);
}

TEST_F(EventTest, TestSize)
{
	ASSERT_EQ(800,e1->getData().size());
	ASSERT_EQ(800,e2->getData().size());
}

TEST_F(EventTest, TestForEach)
{
	unsigned int i = 0;
	for(uint16_t number : e1->getData() )
	{
		i++;
	}
	ASSERT_EQ(800,i);
}

TEST_F(EventTest, TestCopyConstructor)
{
	Event copied1(*e1);
	Event copied2(*e2);

	ASSERT_FALSE(&copied1 == e1);
	ASSERT_FALSE(&copied2 == e2);
	ASSERT_FALSE(&copied1.getData() == &e1->getData());
	ASSERT_FALSE(&copied2.getData() == &e2->getData());
	ASSERT_TRUE(copied1.getData() == e1->getData());
	ASSERT_TRUE(copied2.getData() == e2->getData());
	ASSERT_EQ(42,copied1[1]);
	ASSERT_EQ(1337,copied2[0]);
}

TEST_F(EventTest, TestPolymorphism)
{
	Event* e = new Event(*e1);
	Data<uint16_t>* data = e;
	ASSERT_TRUE(data != nullptr);
	delete data;
}

TEST_F(EventTest, TestPointerToEvent)
{
	//TEST, if unique pointer in Data detects, if another pointer to the Event object is created - it doesn't
	Event* e = e1;
	Event** arr = new Event*[2];
	arr[0] = e1;
	arr[1] = e;
	ASSERT_EQ(e,e1);
	ASSERT_EQ(e->getData(),e1->getData());
	ASSERT_EQ(&e->getData(),&e1->getData());
}

TEST_F(EventTest, TestConstCorrectness)
{
	const Event copy1(*e1);
	Event copy2(*e2);
//	copy1[0]++; //doesn't compile since for a const object, the const method is used
	copy2[0]++;
//	copy2.getData()[1] = 1; //doesn't compile, getData() is a const return
	ASSERT_EQ(1338,copy2[0]);
}

TEST_F(EventTest, TestDriftTime)
{
	unique_ptr<vector<uint16_t>> arr = unique_ptr<vector<uint16_t>>(new vector<uint16_t>(800,2200));
	unique_ptr<vector<uint16_t>> arr2 = unique_ptr<vector<uint16_t>>(new vector<uint16_t>(800,2200));
	(*arr)[50] = 2100;
	Event e(1,move(arr));
	Event e2(2,move(arr2));

	ASSERT_EQ(200,e.getDriftTime());
	ASSERT_EQ(-168,e2.getDriftTime());
}

TEST_F(EventTest, TestNormalized)
{
	unique_ptr<vector<uint16_t>> arr = unique_ptr<vector<uint16_t>>(new vector<uint16_t>(800,1));
	unique_ptr<vector<double>> res;

	Event e(1,move(arr));
	res = e.normalized();

	for(unsigned int i = 0; i < arr->size(); i++)
	{
		ASSERT_DOUBLE_EQ(1/(double)800,(*res)[i]);
	}
}

TEST_F(EventTest, TestAssignment)
{
	Event e(*e1);
	e = *e2;
	ASSERT_EQ(e2->getEventNumber(),e.getEventNumber());
	ASSERT_EQ(e2->getData(),e.getData());
	ASSERT_FALSE(&e2->getData() == &e.getData());
	ASSERT_FALSE(e2 == &e);
	(*e2)[0] = 13378;
	ASSERT_EQ(13378,e2->getData().at(0));
	ASSERT_FALSE(e[0] == 13378);
	ASSERT_FALSE(e[0] == (*e2)[0]);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
