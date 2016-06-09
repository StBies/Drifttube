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

.PHONY :	directories

all: directories prog

prog: $(OBJ)
	$(CC) -o prog.out $^ $(LDFLAGS) $(ROOTLDFLAGS)
	

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(ROOTCFLAGS) -c -o $@ $<
	
directories: ${OBJDIR}

${OBJDIR}:
	$(MKDIR) ${OBJDIR}
	
clean: directories
	rm -r ${OBJDIR}
	rm prog.out
