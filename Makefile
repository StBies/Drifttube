CC			=	g++
CFLAGS		=	-std=c++11 -O3 -fopenmp
ROOTINC		=	-I/opt/root/5.34.00/include
ROOTLIBS	=	-L/opt/root/5.34.00/lib/
ROOTCFLAGS	=	$(shell root-config --cflags)
ROOTLDFLAGS	=	$(shell root-config --libs)
LDFLAGS		=	-fopenmp
SRC			=	$(wildcard src/*.cpp)
OBJ			=	$(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
OBJDIR		=	obj
MKDIR		=	mkdir -p
TESTOBJ		= 	obj/Drifttube.o obj/DataSet.o obj/Event.o obj/Exception.o obj/EventSizeException.o

.PHONY :	directories

all: directories prog test

prog: $(OBJ)
	$(CC) -o prog.out $^ $(LDFLAGS) $(ROOTLDFLAGS)
	

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(ROOTCFLAGS) -c -o $@ $<
	
directories: ${OBJDIR}

test: $(TESTOBJ)
	$(CC) -std=c++11 -I./gtest/include -L./gtest/lib src/unitTests/DrifttubeTest.cpp $(TESTOBJ) -o DrifttubeTest.out -lgtest -lgtest_main -lpthread
	$(CC) -std=c++11 -I./gtest/include -L./gtest/lib src/unitTests/EventTest.cpp $(TESTOBJ) -o EventTest.out -lgtest -lgtest_main -lpthread

${OBJDIR}:
	$(MKDIR) ${OBJDIR}
	
clean: directories
	rm -r ${OBJDIR}
	rm prog.out
