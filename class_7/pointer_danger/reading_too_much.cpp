#include <iostream>
#include <string.h>


int main() {
	int intermediate = 42;
	int shorter[3] = {1, 2, 3};
	int destination[7] = {0, 0, 0, 0, 0, 0, 0};

	/*std::cout << & shorter << " " << & (shorter[1]) << " " << & shorter[2] << std::endl;
	std::cout << (long)(& shorter) - (long)(& intermediate) << " " << sizeof(int) << std::endl;
	std::cout << & intermediate << " " << & destination << std::endl;*/

	memcpy(& destination, shorter, 7*sizeof(int));

	for (int i=0; i<7; i++) { std::cout << destination[i] << std::endl; }

	return 0;
}
