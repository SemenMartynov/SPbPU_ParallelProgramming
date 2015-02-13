CXXFLAGS =	-O0 -g -std=c++11 -pedantic -Werror -Wall  -fmessage-length=0 

OBJS =		correctness.o BTreeNode.o BTree.o gtest-all.o
OBJS2 =		performance.o BTreeNode.o BTree.o

LIBS =		-pthread

TARGET =	correctness
TARGET2 =	performance

gtest-all.o:
	$(CXX) -g -c gtest/gtest-all.cc -o gtest-all.o

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)
	
$(TARGET2):	$(OBJS2)
	$(CXX) -o $(TARGET2) $(OBJS2)

all:	$(TARGET) $(TARGET2)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(OBJS2) $(TARGET2)
