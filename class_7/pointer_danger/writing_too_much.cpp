#include <iostream>
#include <string.h>


int main() {
	int intermediate = 42;
	int shorter[3] = {1, 2, 3};
	int longer[4] = {1, 4, 6, 84};

	/*std::cout << & shorter << " " << & (shorter[1]) << " " << & shorter[2] << std::endl;
	std::cout << (long)(& shorter) - (long)(& intermediate) << " " << sizeof(int) << std::endl;
	std::cout << & intermediate << " " << & longer << std::endl;*/

	memcpy(& shorter, & longer, 4*sizeof(int));

	//for (int i=0; i<7; i++) { std::cout << destination[i] << std::endl; }
	std::cout << intermediate << std::endl;

	return 0;
}
