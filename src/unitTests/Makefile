CC = g++
CFLAGS = -g -lgtest -lgtest_main -lpthread
OBJS = ../../obj/Drifttube.o DrifttubeTest.o

testAll: $(OBJS)
	$(CC) $(CFLAGS) -o testAll  runTests.cpp $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm testAll *.o testAll.xml
