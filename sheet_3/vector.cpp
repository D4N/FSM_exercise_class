#include <cmath>
#include <iostream>

#include "vector.hpp"


cartesian_vector cartesian_vector::operator+ (const cartesian_vector & summand) {
	return cartesian_vector(x + summand.x, y + summand.y, z + summand.z);
}


cartesian_vector cartesian_vector::operator- (const cartesian_vector & summand) {
	return cartesian_vector(x - summand.x, y - summand.y, z - summand.z);
}

cartesian_vector& cartesian_vector::operator+= (const cartesian_vector & summand) {
	x += summand.x;
	y += summand.y;
	z += summand.z;
	return *this;
}

cartesian_vector& cartesian_vector::operator-= (const cartesian_vector & summand) {
	x -= summand.x;
	y -= summand.y;
	z -= summand.z;
	return *this;
}

cartesian_vector& cartesian_vector::operator= (const cartesian_vector & summand) {
	x = summand.x;
	y = summand.y;
	z = summand.z;
	return *this;
}

cartesian_vector& cartesian_vector::operator*= (const double & factor) {
	x *= factor;
	y *= factor;
	z *= factor;
	return *this;
}

cartesian_vector& cartesian_vector::operator/= (const double & factor) {
	x /= factor;
	y /= factor;
	z /= factor;
	return *this;
}


cartesian_vector operator * (const cartesian_vector & vec, const double & value) {
	return cartesian_vector(value*vec.x, value*vec.y, value*vec.z);
}

cartesian_vector operator * (const double & value, const cartesian_vector & vec) {
	return cartesian_vector(value*vec.x, value*vec.y, value*vec.z);
}

cartesian_vector operator / (const cartesian_vector & vec, const double & value) {
	return cartesian_vector(vec.x/value, vec.y/value, vec.z/value);
}

/*cartesian_vector operator / (const double & value, const cartesian_vector & vec) {
	return cartesian_vector(value/vec.x, value/vec.y, value/vec.z);
}*/




double cartesian_vector::operator * (const cartesian_vector & factor) {
	return x*factor.x + y*factor.y + z*factor.z;
}


double cartesian_vector::norm () {
	return sqrt(x*x + y*y + z*z);
}

double cartesian_vector::norm_squared () {
	return x*x + y*y + z*z;
}


std::ostream &operator<< (std::ostream & stream, cartesian_vector vec) {
	//return stream << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
	return stream << vec.x << ' ' << vec.y << ' ' << vec.z;
}
