CFLAGS=-c -Wall -std=c++11 -O3

all: simulation

simulation: n-body.o vector.o body.hpp main.o
	g++ -Wall -std=c++11 -O3 main.o n-body.o vector.o -o simulation.out

main.o: main.cpp n-body.o
	g++ $(CFLAGS) main.cpp

n-body.o: n-body.cpp n-body.hpp vector.o
	g++ $(CFLAGS) n-body.cpp

vector.o: vector.cpp vector.hpp
	g++ $(CFLAGS) vector.cpp

clean:
	rm -rf main.o n-body.o vector.o simulation.out
