/* File vector.hpp */
#ifndef FILE_VECTOR_HPP
#define FILE_VECTOR_HPP

/*
 * Cartesian vector implementation:
 *
 * create a vector simply using: cartesian_vector vec;
 * 	vec is then empty (the components are undefined) 
 *
 * or: cartesian_vector vec (a, b, c)
 * 	where a, b and c are doubles
 *
 * you can add and subtract vectors from each other or multiply them to get the scalar product
 *
 * you can multiply vectors with doubles to get the vector times the number
 *
 * the absolute value and the absolute value squared are accessible via the member functions:
 *	 norm() and norm_squared()
 *
 * the following assignment operators are defined:
 *	+=, -=, = (for vectors); *=, /= (for doubles)
 *
 * the << operator from iostream is overloaded, so that you can print your vectors with cout << cartesian_vector
 */

#include <iostream>

class cartesian_vector {
	public:
		double x, y, z;

		// constructors:
		cartesian_vector () {}
		cartesian_vector (double a,double b, double c) : x(a), y(b), z(c) {}

		// addition operator
		cartesian_vector operator + (const cartesian_vector &);
		cartesian_vector operator - (const cartesian_vector &);
		cartesian_vector& operator += (const cartesian_vector &);
		cartesian_vector& operator -= (const cartesian_vector &);
		cartesian_vector& operator = (const cartesian_vector &);
		cartesian_vector& operator *= (const double &);
		cartesian_vector& operator /= (const double &);


		// scalar product
		double operator * (const cartesian_vector &);


		// calculation of the norm:
		double norm_squared();
		double norm();

		// overload the << operator for iostream, this enables us to print the vector directly
		friend std::ostream &operator << (std::ostream & stream, cartesian_vector vec);
};


cartesian_vector operator * (const double & value, const cartesian_vector & vec);
cartesian_vector operator * (const cartesian_vector & vec, const double & value);

//cartesian_vector operator / (const double & value, const cartesian_vector & vec);
cartesian_vector operator / (const cartesian_vector & vec, const double & value);


#endif /* !FILE_VECTOR_HPP */
