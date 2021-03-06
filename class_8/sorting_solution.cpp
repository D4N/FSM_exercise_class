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

void print_vector(std::vector<double> to_print) {
	for (double value : to_print) {
		std::cout << value << " ";
	}
	std::cout << std::endl;
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

	// print the original array
	std::cout << "Original array:" << std::endl; print_vector(random_values);

	// Sorting using the STL is quite easy:
	std::sort(random_values.begin(), random_values.end());

	// print the sorted array
	std::cout << "Sorted array:" << std::endl; print_vector(random_values);

	// You can do funky stuff and provide your own comparison function.
	// This can be usefull to compare 3D vectors for instance.
	// We will now simply reverse the sorting by defining the comparison exactly the other way around.
	std::sort(random_values.begin(), random_values.end(), [](double i, double j) { return (i > j); });

	// print the sorted array
	std::cout << "Reversely sorted array:" << std::endl; print_vector(random_values);

	return 0;
}
