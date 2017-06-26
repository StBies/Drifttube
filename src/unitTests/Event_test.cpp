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
		std::unique_ptr<std::array<uint16_t,800>> a(new std::array<uint16_t,800>);
		std::unique_ptr<std::array<uint16_t,800>> b(new std::array<uint16_t,800>);
		a->fill(42);
		b->fill(1337);
		e1 = new Event(42,move(a));
		e2 = new Event(1337,move(b));
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
	}

protected:
	Event* e1;
	Event* e2;
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
}

TEST_F(EventTest, TestEventSize)
{
	ASSERT_EQ(800,e1->getData().size());
	ASSERT_EQ(800,e2->getData().size());
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
	for(size_t i = 0; i < e->getData().size(); i++)
	{
		ASSERT_EQ((*e)[i],(*e1)[i]);
	}
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
	unique_ptr<array<uint16_t,800>> arr = unique_ptr<array<uint16_t,800>>(new array<uint16_t,800>);
	unique_ptr<array<uint16_t,800>> arr2 = unique_ptr<array<uint16_t,800>>(new array<uint16_t,800>);
	arr->fill(2200);
	arr2->fill(2200);
	(*arr)[50] = 2100;
	Event e(1,move(arr));
	Event e2(2,move(arr2));



	ASSERT_EQ(200,e.getDriftTime());
	ASSERT_EQ(-168,e2.getDriftTime());
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
