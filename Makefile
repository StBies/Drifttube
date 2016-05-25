CC		=	g++
CFLAGS		=	-std=c++11 -O3
ROOTINC		=	-I/opt/root/5.34.00/include
ROOTLIBS	=	-L/opt/root/5.34.00/lib/
ROOTCFLAGS	=	$(shell root-config --cflags)
ROOTLDFLAGS	=	$(shell root-config --libs)
OBJ		=	main.o

SRC=$(OBJ:%.o:%.cpp)

%.o: %.cpp
	$(CC) $(CFLAGS) $(ROOTCFLAGS) -c $<

prog: $(OBJ)
	$(CC) -o prog.out $(OBJ) $(LDFLAGS) $(ROOTLDFLAGS)
	rm *.o
