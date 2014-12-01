/* FILE BODY.HPP */
#ifndef FILE_BODY_HPP
#define FILE_BODY_HPP

#include "vector.hpp"

class body {
	public:
		cartesian_vector position, velocity;
		double mass;

		// constructors
		body () {}
		body (cartesian_vector x, cartesian_vector v, double mass) : position(x), velocity(v), mass(mass) {}
};

#endif /* FILE_BODY_HPP */
