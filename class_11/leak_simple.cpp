#include <iostream>
#include <random>
#include <functional>

static const int target = 1000;


void simple_leak(int value) {

	int * values = new int[10];
	values[0] = value;

	std::cout << values[0] << std::endl;
}

void bad_memory_free(int value) {

	int * values = new int[10];
	values[0] = value;

	std::cout << values[0] << std::endl;

	delete values;
}


std::function<int(int)> function_builder(int value) {
	if (value < target/10.) {
		return [](int x) { return x*2; };
	}
	else if (value < target/4.) {
		return [](int x) { return x*4; };
	}
	else if (value < target/2.) {
		return [](int x) { return x/2; };
	}
	else if (value < 2.*target/3.) {
		return [](int x) { return x/4; };
	}
	else if (value < 9.*target/10.) {
		return [](int x) { return x/3; };
	}
	else { return nullptr; }
}


double leak_or_no_leak(int count) {

	int * values = new int[count];
	std::function<int(int)> weighter = function_builder(count);

	std::random_device rnd;

	for (int i=0; i<count; i++) {
		values[i] = weighter(rnd());
	}

	double result = 0;
	for (int i=0; i<count; i++) {
		result += values[i];
	}

	return result/count;
}

void mean_sum(int runs) {

	std::random_device rnd;
	for (int i=0; i<runs; i++) {
		int count = 900.*rnd()/rnd.max();

		try { leak_or_no_leak(count); }
		catch (...) { }
	}

}


int main() {
	//simple_leak(10);
	//bad_memory_free(10);
	mean_sum(100);

	return 0;
}
