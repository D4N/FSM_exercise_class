#include <iostream>

#include "vector.hpp"


using namespace std;


int main() {

	cartesian_vector vec;
	cout << vec << endl;

	cartesian_vector position1 (1., 1., 1.);
	cartesian_vector position2 (-1., 2., 0.);

	cartesian_vector position3 = position1 + position2;

	cout << position1 << " + " << position2 << " = " << position3 << endl;
	cout << position1 << " - " << position2 << " = " << position1 - position2 << endl;

	cout << "norm: " << position3.norm() << endl;
	cout << "norm squared: " << position3.norm_squared() << endl;

	cout << "scalar product: " << position1 << " * " << position3 << " = " << position1 * position3 << endl;

	cout << "scalar multiplication: 3.0 * " << position3 << " = " << position3*3. << endl;
	cout << "scalar multiplication: 3.0 * " << position3 << " = " << 3.*position3 << endl;

	return 0;
}
