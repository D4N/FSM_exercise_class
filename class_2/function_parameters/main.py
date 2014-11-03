# -*- coding: utf-8 -*-

from __future__ import division, print_function

import numpy as np

"""
Passing functions as parameters in Python is very easy, since everything in Python is of the same type.

Thus if you have defined a function with the name 'my_func', you pass it as a parameter by writing my_func into the parameter list of a function call. Can't get any simpler than this.

How is this possible?
Internally all Python objects are the same, but have different capabilities. This means you could give a function that requires a parameter to be function instead of the function a list. The Python interpreter will accept this during the syntax check, but when the line of code is executed, the interpreter will complain (it will throw an exception), because you cannot call a list.

You can find some examples in the following lines.
"""

def function_caller(value, function):
	"""
	This function demonstrates how to pass a function as a parameter. It returns the value of function for the given parameter:
	>>> function_caller(1., lambda x: x*x)
	1.0

	If function is not callable, an exception will be thrown:
	>>> function_caller(-1, [-1, -2])
	Traceback (most recent call last):
	 ...
	TypeError: 'list' object is not callable

	"""
	return function(value)

"""
You can define functions in the usuall way, or in a short form as one-liners: lambda expressions.
The syntax is the following:

function_name = lambda parameters: return_value

lambda expressions can be defined in a function call to shorten your code. For instance this:

def my_func(x):
	return x*np.log(x) - np.sin(x)
function_caller(np.linspace(0, 100, 1000), my_func)

becomes this:

function_caller(np.linspace(0, 100, 1000), lambda x: x*np.log(x) - np.sin(x))


You can of course pass several parameters to a lambda expression:
my_func = lambda t, x: t**(-1.*x)


A few examples are given in the following lines:
"""

def test_function_1(x):
	return x*np.sin(x)

test_function_2 = lambda x: x*np.sin(x)



if __name__ == '__main__':

	print(function_caller(.5, test_function_1))
	print(function_caller(.5, test_function_2))
	print(function_caller(0., np.cos))
	print(function_caller(1., lambda t: np.exp(-t)))
