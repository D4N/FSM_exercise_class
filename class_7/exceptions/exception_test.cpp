#include <iostream>
#include <exception>
#include <string>



static const int max_element_count = 100;

class list_element {
	public:
		int value;
		list_element * next;
};


class premature_list_end_exception : public std::exception {
	public:
		const char * what () const throw () { return "List end reached prematurely."; }
};


namespace ERRORS {
	enum ERROR_CODES { LIST_TOO_LONG = 0, LIST_END_NOT_REACHED, LIST_END_REACHED_PREMATURELY };

	const char * error_description(ERROR_CODES error) {
		switch (error) {
			case LIST_TOO_LONG:
				return "The list is longer than the specified limit.";
			case LIST_END_NOT_REACHED:
				return "The list end was not reached.";
			case LIST_END_REACHED_PREMATURELY:
				return "The list is shorter than the number of elements you specified.";
			default:
				return "Unknown error";
		}
	}
}


void build_zeroed_list(int element_count, list_element * first_node) {
	/*
	 * Initializes an empty list with element_count elements, with value 0.
	 * The list can be at most 100 elements long, else an exception is thrown.
	 */

	if (element_count > max_element_count) {
		throw 1;
	}

	list_element * current = first_node;

	for (int i=0; i<element_count-1; i++) {
		list_element * next_element = new list_element;
		current->next = next_element;
		current->value = 0;
		current = current->next;
	}
	current->next = nullptr;

}


void print_list_elements(int list_length, list_element * first_node) {

	list_element * current = first_node;

	for (int i=0; i<list_length; i++) {
		if (current == nullptr) {
			throw new premature_list_end_exception;
		}
		std::cout << "element number: " << i+1 << ", has value: " << current->value << std::endl;
		current = current->next;
	}

	if (current != nullptr) {
		throw 2;
	}
}


int get_list_sum(int list_length, list_element * first_node)
{
	if (list_length == 42) {
		throw new premature_list_end_exception;
	}
	if (list_length > max_element_count) {
		throw ERRORS::LIST_TOO_LONG;
	}

	list_element * current = first_node;
	int sum = 0;

	for (int i=0; i<list_length; i++) {
		if (current == nullptr) {
			throw ERRORS::LIST_END_REACHED_PREMATURELY;
		}
		sum += current->value;
		current = current->next;
	}

	if (current != nullptr) {
		throw ERRORS::LIST_END_NOT_REACHED;
	}

	return sum;
}

int main() {

	int element_count = 0;
	std::cout << "Number of elements in the list: ";
	std::cin >> element_count;

	list_element * first = new list_element;

	try {
		build_zeroed_list(element_count, first);
	}
	catch (int e) {
		std::cerr << "Too many elements specified" << std::endl;
	}

	try {
		print_list_elements(element_count, first);
	}
	catch (premature_list_end_exception * exc) {
		std::cerr << exc->what() << std::endl;
	}
	catch (int exc) {
		std::cerr << "List not empty after printing" << std::endl;
	}



	list_element * other = new list_element;

	try {
		build_zeroed_list(80, other);
	}
	catch (int e) {
		std::cerr << "Too many elements specified" << std::endl;
	}

	try {
		get_list_sum(70, other);
	}
	catch(ERRORS::ERROR_CODES error) {
		std::cerr << ERRORS::error_description(error) << std::endl;
	}

	try {
		get_list_sum(90, other);
	}
	catch(ERRORS::ERROR_CODES error) {
		std::cerr << ERRORS::error_description(error) << std::endl;
	}

	try {
		get_list_sum(120, other);
	}
	catch(ERRORS::ERROR_CODES error) {
		std::cerr << ERRORS::error_description(error) << std::endl;
	}

	try {
		get_list_sum(42, other);
	}
	catch(ERRORS::ERROR_CODES error) {
		std::cerr << ERRORS::error_description(error) << std::endl;
	}
	catch(...){
		std::cerr << "A different error occured." << std::endl;
	}


	return 0;
}
