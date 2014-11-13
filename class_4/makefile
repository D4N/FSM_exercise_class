CFLAGS=-c -Wall -std=c++11

all: main

main: tree_test.o binary_tree.o
	g++ -Wall -std=c++11 tree_test.o binary_tree.o -o tree.out

tree_test.o: tree_test.cpp
	g++ $(CFLAGS) tree_test.cpp

binary_tree.o: binary_tree.cpp binary_tree.hpp
	g++ $(CFLAGS) binary_tree.cpp

clean:
	rm -rf binary_tree.o tree_test.o tree.out
