#include <iostream>

using namespace std;


void address_demonstration() {
	// print out the address of an int:
	int number = 10;
	cout << number << ' ' << & number << endl;

	// save the address
	int other_number = 11;
	int * address = & other_number;

	cout << address << endl;
}

void dereference_demonstration() {
	int half_the_truth = 21;
	int * address = & half_the_truth;

	cout << * address << endl;

	// modify the value
	(* address) *= 2;

	cout << half_the_truth << endl;
}

void riddle() {

	// declare and define variables and respective pointers
	int number1 = 2, number2 = 4;
	int * pointer1, * pointer2;

	pointer1 = & number1;
	pointer2 = & number2;


	// evil conversions
	* pointer1 = 8;
	* pointer1 = * pointer2;
	pointer1 = pointer2;
	* pointer1 = 16;

	//solve the riddle
	//cout << number1 << ' ' << number2 << endl;
}

void arrays_n_pointers() {
	int arr[6] = { 1, 2, 3, 4, 5, 9 };
	short other_arr[6] = { 1, 2, 3, 4, 5, 9 };

	// this is valid:
	int * ptr = arr;

	//for (int i=0; i<6; i++) { cout << arr[i] << ' ' << * (ptr++) << endl; }
	for (int i=0; i<6; i++) { cout << arr[i] << ' ' << * (ptr+i) << endl; }

	cout << endl;
	// as pointers and arrays are the same, you can use the syntax for both:
	for (int i=0; i<6; i++) { cout << * (arr + i) << ' ' << ptr[i] << endl; }

	// beware ++ or +1 does not mean memory address + 1 but next memory cell considering the current type!
	cout << arr << ' ' << arr + 1 << endl;
	cout << other_arr << ' ' << other_arr + 1 << endl;
	cout << other_arr << ' ' << ((int *)other_arr) + 1 << endl;

}

void useful (int number, int * squared, int * cubed) {
	* squared = number * number;
	* cubed = (* squared)* number;
}

void call_by_ptr1() {
	int squared = 0, cubed = 0;
	int * ptr1 = & squared;
	int * ptr2 = & cubed;

	cout << squared << ' ' << cubed << endl;
	useful (10, ptr1, ptr2);
	cout << squared << ' ' << cubed << endl;
}

void call_by_ptr2() {
	int * squared = new int;
	int * cubed  = new int;

	// squared and cubed are now null pointers (they are equal and point to the value 0)
	cout << *squared << ' ' << *cubed << endl;
	useful (20, squared, cubed);
	cout << *squared << ' ' << *cubed << endl;
}

void useful_new(int number, int & squared, int & cubed) {
	squared = number*number;
	cubed = squared*number;
}

void call_by_ref1() {
	int squared = 0, cubed = 0;

	cout << squared << ' ' << cubed << endl;
	useful_new (30, squared, cubed);
	cout << squared << ' ' << cubed << endl;
}

void call_by_ref2() {
	int * squared = new int;
	int * cubed  = new int;

	cout << *squared << ' ' << *cubed << endl;
	useful_new (40, *squared, *cubed);
	cout << *squared << ' ' << *cubed << endl;
}


class list_node {
	public:
		double value;
		list_node () { };
		list_node (double val) { value = val; }
		list_node * next;
};

void show_list() {
	list_node node1(1.);
	list_node node2(2.);
	list_node node3(4.);
	list_node node4(8.);

	node1.next = & node2;
	node2.next = & node3;
	node3.next = & node4;
	node4.next = 0;	// terminate list, important!

	// traverse list:
	list_node * ptr = & node1;
	while (ptr != 0) {
		cout << (*ptr).value << endl;
		ptr = (*ptr).next;
	}
}


int main() {
	//address_demonstration();

	//dereference_demonstration();

	//riddle();

	//arrays_n_pointers();

	//call_by_ptr1();
	//call_by_ptr2();

	//call_by_ref1();
	//call_by_ref2();

	//show_list();

	return 0;
}
