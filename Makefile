CC			=	g++
CFLAGS		=	-std=c++11 -O3 -fopenmp
ROOTCFLAGS	=	$(shell root-config --cflags)
ROOTLDFLAGS	=	$(shell root-config --libs)
TESTINC		=	-I./gtest/include
TESTLIB		=	-L./gtest/lib
LDFLAGS		=	-fopenmp
TESTLDFLAGS = 	-lgtest -lgtest_main -lpthread
SRC			=	$(wildcard src/*.cpp)
TESTSRC		=	$(wildcard src/unitTests/*.cpp)
TESTFILES	=	$(notdir $(TESTSRC:.cpp=))
TESTEDOBJS	=	$(addprefix obj/,$(notdir $(TESTSRC:_test.cpp=.o)))
TESTEDOBJS 	+= 	obj/Exception.o obj/EventSizeException.o
OBJ			=	$(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
OBJDIR		=	obj
MKDIR		=	mkdir -p

.PHONY :	directories

all: directories prog test

prog: $(OBJ)
	$(CC) -o prog.out $^ $(LDFLAGS) $(ROOTLDFLAGS)
	
obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(ROOTCFLAGS) -c -o $@ $<
	
%_test.out:  $(TESTOBJS)
	
test: $(TESTSRC) $(TESTEDOBJS)
	for i in $(TESTSRC); do \
		$(CC) $(CFLAGS) $(TESTINC) $(TESTLIB) $$i $(TESTEDOBJS) -o $(notdir $$i.out) $(LDFLAGS) $(TESTLDFLAGS); \
	done	
	
directories: ${OBJDIR}
	
${OBJDIR}:
	$(MKDIR) ${OBJDIR}
	
testTest:
	@echo $(TESTFILES)
	
clean: directories
	rm -r ${OBJDIR}
	rm prog.out
