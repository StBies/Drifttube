CC			=	g++
CFLAGS		=	-std=c++11 -O3 -fopenmp
ROOTCFLAGS	=	$(shell root-config --cflags)
ROOTLDFLAGS	=	$(shell root-config --libs)
LDFLAGS		=	-fopenmp
TESTLDFLAGS = 	-lgtest -lgtest_main -lpthread
SRC			=	$(wildcard src/*.cpp)
TESTSRC		=	$(wildcard src/unitTests/*.cpp)
TESTOBJS	=	$(addprefix obj/,$(notdir $(TESTSRC:.cpp=.o)))
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
	
	
	
directories: ${OBJDIR}
	
${OBJDIR}:
	$(MKDIR) ${OBJDIR}
	
testTest:
	@echo $(TESTEDOBJS)
	
clean: directories
	rm -r ${OBJDIR}
	rm prog.out
