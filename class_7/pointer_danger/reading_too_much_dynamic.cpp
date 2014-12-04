#include <iostream>
#include <string.h>


int main() {
	int * intermediate = new int;
	* intermediate = 42;
	int * shorter = new int[3];
	for (int i=1; i<4; i++) { * (shorter + i-1) = i; }
	int * destination = new int[7];
	for (int i=0; i<7; i++) { * (destination + i) = 0; }

	/*std::cout << & shorter << " " << & (shorter[1]) << " " << & shorter[2] << std::endl;
	std::cout << (long)(& shorter) - (long)(& intermediate) << " " << sizeof(int) << std::endl;
	std::cout << & intermediate << " " << & destination << std::endl;*/

	memcpy(& destination, shorter, 7*sizeof(int));

	for (int i=0; i<7; i++) { std::cout << destination[i] << std::endl; }

	return 0;
}
