#include <iostream>
#include <math.h>

using namespace std;

/*
 * This file shows you how to create functions, that take other functions as parameters.
 * This is usually achieved using function pointers, though there are other ways.
 *
 * A function pointer points in contrast to 'ordinary' pointers to functions. It is defined in the following way:
 *	return_type (* function_pointer) (parameter_type_1, parameter_type_2, ..., parameter_type_N) = function_to_point_to;
 *
 * for instance to create a pointer to the sine function from math.h you would write:
 *	double (* pointer_to_sine) (double) = sin;
 *
 *
 * So how do you use a function pointer? You just have to de-reference the pointer and pass it the parameters. For our generic example it would be:
 *	return_type variable = (* function_pointer) (parameter_1, parameter_2, ..., parameter_N);
 * and for our pointer to the sine function:
 *	y = (* pointer_to_sine) (x);
 * where x and y are double variables. The last line would be equivalent to:
 *	y = sin(x);
 *
 *
 * If you now want a function to take a function pointer as a parameter, you have to simply include it in the definition of the function.
 * For example:
 *
 *	void function_with_function_pointer(double (*function_pointer)(double));
 *
 * You could call this function with with our pointer to the sine function,
 * or with any other function that takes one double as a parameter and returns a double.
 *
 * Enough theory, let's take a look on some examples.
 */


// We will use the following function for testing, it does nothing else than to call the function you pass it with the given parameter and print the result to standard output.
void function_caller(double (*func)(double), double parameter) {
	cout << (*func)(parameter) << endl;
}

// Very complex self-defined function.
double func(double x) {
	return x*x;
}


int main() {
	double (*function_pointer1) (double) = func;

	/*
	 * Fortunately you do not have to create a function pointer every time you want to call a function that takes a function pointer as a parameter.
	 * You can simply give it the function name. Therefore the following two lines do the same:
	 */
	function_caller(*function_pointer1, 1.0);
	function_caller(func, 1.0);

	// You can of course use functions defined in headers, in this case in math.h
	function_caller(exp, 1.0);

	/*
	 * If your functions are simple, but not simple enough to be already implemented,
	 * then you might want to have a shorter version how to define functions.
	 * Especially if you could write this definition in the function call.
	 *
	 * Consider the following example: You have a function that prints the values of a function for all values in an array:
	 * 	void print_function_values(double array[], double (* function) (double));
	 * And lets assume you want to invoke this function often with different functions as parameters. If you would define the functions which values you want to print out the usual way, you'll end up with a big bunch of function definitions. These are tedious to write and if you don't give them good names, you'll quickly forget which is which (for instance with names like func1, func2, etc). Also if you only need them to print out their values, then you do not really need them for the rest of your program.
	 * A more convenient way would be something like this:
	 *	print_function_values(values, function: return x*sin(x) );
	 * i.e. you define in the call of print_function_values the function to use. This is very useful if you'll never again need x*sin(x)
	 *
	 * 
	 * For this so called lambda expressions have been included in C++ 11.
	 * Please note: the following code does not compile in a compiler that does not support C++ 11! Gcc has to be started with -std=c++11 to compile.
	 *
	 * A lambda expression does exactly the above simplification in the following syntax:
	 * [](parameters) -> return_type { function_body }
	 *
	 * This would be equivalent to:
	 * return_type some_random_function_name (parameters)
	 *	{ function_body }
	 *
	 * Advanced: The '[]' in the beginning of the lambda expression can be used to define how to capture the parameters. E.g. by value or by reference.
	 * For further information, check out the Wikipedia page: https://en.wikipedia.org/wiki/Lambda_%28programming%29
	 *
	 */

	// We can let our function caller invoke x*cos(x), without having to define it explicitly:
	function_caller([](double x) { return x*cos(x); }, 1.0);

	// You can always save a lambda expression in a variable name using the type auto:
	auto my_func = [](double x) { return x*tan(x); };
	/* This is equivalent to:
	double my_func (double x) {
		return x*tan(x);
	}
	*/

	// And you can of course use the previously saved lambda expression to pass it as a function pointer: 
	function_caller(my_func, 1.0);


	return 0;
}
