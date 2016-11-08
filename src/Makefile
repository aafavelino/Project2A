CC = g++

CFLAGS = -g -c -Wall -pedantic -std=c++14 -DDEBUGMODE
#CFLAGS = -O2 -c -Wall -pedantic -std=c++14

LFLAGS = -g
#LFLAGS = -O2

INC1 = .
INC2 = ../cdt
INC3 = ../common

INCS = -I$(INC1) -I$(INC2) -I$(INC3)

LIB1 = ../lib

LIBS = -L$(LIB1) -lm -lCDT

OBJS = main.o PDSampler.o

project2a: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o project2a $(LIBS)
	mv project2a ../bin/.

main.o:	$(INC2)/Cdt.h $(INC2)/DtPoint.h $(INC1)/PDSampler.h \
	$(INC2)/FaceVisitor.h $(INC1)/SamplePoint.h \
	$(INC1)/MyCylinder.h $(INC1)/main.cpp
	$(CC) $(CFLAGS) $(INC1)/main.cpp $(INCS)

PDSampler.o: $(INC1)/Surface.h $(INC1)/SamplePoint.h \
	$(INC3)/Random.h \
	$(INC1)/PDSampler.h $(INC1)/PDSampler.cpp 
	$(CC) $(CFLAGS) $(INC1)/PDSampler.cpp $(INCS)

clean:
	rm -fr *.o *~

realclean:
	rm -fr *.o *~ ../bin/project2a
