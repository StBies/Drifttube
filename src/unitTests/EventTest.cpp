#include "../Event.h"
#include <gtest/gtest.h>

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
	ASSERT_EQ(1337,e1->getData().at(5));
}

TEST_F(EventTest, TestEventSize)
{
	ASSERT_EQ(800,e1->getData().size());
	ASSERT_EQ(800,e2->getData().size());
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
