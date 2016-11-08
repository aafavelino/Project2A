CC = g++
AR = ar

CFLAGS = -g -c -Wall -pedantic -std=c++14 -DDEBUGMODE
#CFLAGS = -O3 -c -Wall -pedantic -std=c++14

INC1 = .

INCS = -I$(INC1)

OBJ = Cdt.o FaceVisitor.o TriangulationVisitor.o

LIB = libCDT.a

Cdt.o: 	$(INC1)/DtPoint.h $(INC1)/Visitor.h \
	$(INC1)/Vertex.h $(INC1)/Edge.h $(INC1)/Quadedge.h \
	$(INC1)/Face.h $(INC1)/Predicates.h \
        $(INC1)/Cdt.h $(INC1)/Cdt.cpp 
	$(CC) $(CFLAGS) $(INC1)/Cdt.cpp $(INCS)

FaceVisitor.o: 	$(INC1)/Visitor.h $(INC1)/Cdt.h \
		$(INC1)/Face.h $(INC1)/Edge.h \
		$(INC1)/Quadedge.h $(INC1)/Vertex.h \
		$(INC1)/FaceVisitor.h $(INC1)/FaceVisitor.cpp 
	$(CC) $(CFLAGS) $(INC1)/FaceVisitor.cpp $(INCS)


TriangulationVisitor.o: $(INC1)/Visitor.h $(INC1)/RegionVisitor.h \
			$(INC1)/Cdt.h $(INC1)/Face.h $(INC1)/Edge.h \
			$(INC1)/Quadedge.h $(INC1)/Vertex.h \
			$(INC1)/TriangulationVisitor.h $(INC1)/TriangulationVisitor.cpp 
	$(CC) $(CFLAGS) $(INC1)/TriangulationVisitor.cpp $(INCS)

all: $(OBJ)

lib:	$(INC1)/Visitor.h $(INC1)/Predicates.h $(INC1)/DtPoint.h \
	$(INC1)/Vertex.h $(INC1)/Edge.h $(INC1)/Quadedge.h \
	$(INC1)/Face.h $(INC1)/RegionVisitor.h $(INC1)/FaceVisitor.h \
	$(INC1)/FaceVisitor.cpp $(INC1)/TriangulationVisitor.h \
	$(INC1)/TriangulationVisitor.cpp $(INC1)/Cdt.h $(INC1)/Cdt.cpp

	$(AR) rc $(LIB) $(OBJ)
	ranlib $(LIB)
	mv $(LIB) ../lib

clean:
	rm -fr *~ *.o

realclean:
	rm -fr *~ *.o ../lib/libCDT.a
