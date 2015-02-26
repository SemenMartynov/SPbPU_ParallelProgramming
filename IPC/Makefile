CXXFLAGS =	-O2 -g -std=c++11 -pedantic -Werror -Wall  -fmessage-length=0 

OBJS =		main.o BTreeNode.o BTree.o

LIBS =		-pthread

TARGET =	IPC

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)
	
all:	$(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
