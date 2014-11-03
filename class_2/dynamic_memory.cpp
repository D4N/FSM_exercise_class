#include <iostream>

using namespace std;


/*
In some cases it is usefull to define variables according to user input. This is problematic as you don't know the user input at compile time and thus cannot define your variable during the writing of your program. Instead you have to dynamically allocate it on the so-called heap.

This could have been used on exercise sheet 1 to let the user decide how many points to calculate for the Kepler orbit.


How do you allocate variables on the heap?
Well, this is a bit more complicated. As the heap is managed by the operating system during the running of your program, you actually just get a pointer to your dynamically allocated space in RAM. This is achieved in the following way:
	variable_type * variable_name = new variable_type;

Now you can use the pointer to do all your operations, just keep in mind that it is a pointer (this usually means that you just have to write an asterisk in front of the variable name).


Is there something else?
YES! After you are done using your variable, you have to delete it! And, you guessed it, this is done using the keyword 'delete':
delete variable_name;
If you do not do this, you create a memory leak!

Why should you do this?
If you do not delete a variable on the heap, the variable will stay there occupying space, until your program terminates. For short programs this is not an issue. But consider a large simulation, which runs for weeks and calls some function which does not delete its variables a thousand times every second. Thereby you can quickly run out of RAM, even if you do no longer need most of the allocated space.

Shouldn't the computer know when to delete my old variables?
It is actually not easy to guess when a variable is no longer needed and with the usage of pointers variables can be used after the function that allocated it, already terminated. Some higher level programming languages have so-called garbage collectors, which do this for you, but this costs you speed on the other hand.


In the following you can find a simple example that shows you how to allocate variables and arrays.
*/


int main() {
	// Ask the user for the length of an array and allocate it on the heap
	int n;
	cin >> n;
	double * arr = new double[n];

	// fill the array with the numbers from 0 to n-1
	for (int i=0; i<n; i++) {
		// you do not have to de-reference the array, as arrays are already pointers themselves
		arr[i] = i;
	}

	// print the array contents
	for (int i=0; i<n; i++) {
		cout << arr[i] << endl;
	}

	// Caution! Arrays are deleted a bit differently from the heap, don't forget the '[]'!
	delete[] arr;


	// Now the same for numbers:
	double * num = new double;
	cout << "number: ";
	cin >> * num;
	cout << * num << endl;
	delete num;


	/*
	So what to do if you want to create an array which length you do not know? (e.g. a simulation that runs with adaptive time steps)

	There are different solutions, but usually you have to either
	1. write your data directly to a file/print to standard output to avoid having to save them
	or
	2. use some higher level data containers like std::list or std::vector
	*/

	return 0;
}
