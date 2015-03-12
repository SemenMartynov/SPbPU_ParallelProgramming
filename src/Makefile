CXXFLAGS =	-O2 -g -std=c++11 -pedantic -Wall -Werror -fmessage-length=0 

OBJS =		BTreeNode.o BTree.o

all:	tests naive pthreads openmp cpp11 ipc

BTreeNode.o:
	$(CXX) $(CXXFLAGS) -c btree/BTreeNode.cpp -o BTreeNode.o

BTree.o:
	$(CXX) $(CXXFLAGS) -c btree/BTree.cpp -o BTree.o

gtest-all.o:
	$(CXX) -g -c gtest/gtest-all.cc -o gtest-all.o

tests: $(OBJS) gtest-all.o tests.o
	$(CXX) -o tests $(OBJS) gtest-all.o tests.o -pthread

naive: $(OBJS) naive.o
	$(CXX) -o naive $(OBJS) naive.o
	
pthreads: $(OBJS) pthreads.o
	$(CXX) -o pthreads $(OBJS) pthreads.o -pthread

openmp.o:
	$(CXX) $(CXXFLAGS) -fopenmp -c openmp.cpp -o openmp.o
	
openmp: $(OBJS) openmp.o
	$(CXX) -fopenmp -o openmp $(OBJS) openmp.o
	
cpp11: $(OBJS) cpp11.o
	$(CXX) -o cpp11 $(OBJS) cpp11.o -pthread

ipc: $(OBJS) ipc.o
	$(CXX) -o ipc $(OBJS) ipc.o -pthread
	
.PHONY: clean
clean:
	rm -f *.o
	rm -f tests naive pthreads openmp cpp11 ipc
	