CXXFLAGS =	-O2 -g -std=c++11 -pedantic -Wall  -fmessage-length=0 -fopenmp

OBJS =		main.o BTreeNode.o BTree.o

LIBS =		-pthread -fopenmp

TARGET =	openmp

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
