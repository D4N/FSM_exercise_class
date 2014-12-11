#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <sstream>


std::vector<double> create_array_of_random_values(int value_count) {
	/*
	 * This function creates a std::vector and fills it with value_count random values between 0 and 1.
	 */

	std::random_device rd;
	std::vector<double> random_values;

	for (int i=0; i<value_count; i++) {
		double current_value = rd()/(double)rd.max();
		random_values.push_back(current_value);
	}

	return random_values;
}


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: ./sorting value_count" << std::endl;
		return 1;
	}

	// get the given parameter as an int
	std::istringstream iss(argv[1]);
	int value_count = 0;
	iss >> value_count;

	std::vector<double> random_values = create_array_of_random_values(value_count);

	std::cout << "Original array:" << std::endl;
	for (double value : random_values) {
		std::cout << value << " ";
	}
	std::cout << std::endl;

	return 0;
}
