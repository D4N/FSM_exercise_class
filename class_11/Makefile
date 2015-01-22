leak: leak_simple.out

leak_simple.out: leak_simple.cpp
	g++ -Wall -std=c++11 leak_simple.cpp -o leak_simple.out

valgrind: leak_simple.out
	valgrind --leak-check=full --show-leak-kinds=all ./leak_simple.out

clean:
	rm leak_simple.out
