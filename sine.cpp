#include <iostream>
#include <math.h>
#define count 1024

int main () {

	long double help[count];
	long double n;

	for (n = 1; n <= count; n++)
		help[int(n-1)] = n / 3.1415;

	for (n = 0; n < count; n++)
		std::cout << sin(help[int(n)]) << std::endl;
}
