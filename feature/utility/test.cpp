#include "utility.hpp"

#include <ctime>
#include <iostream>

int main()
{
	int x = 9, y = 10;
	std::clock_t begin = std::clock();
	for (int i = 1; i < 1000000; ++i)
		eterfree::swap(x, y);
	std::clock_t end = std::clock();
	std::cout << x << ' ' << y << std::endl;
	std::cout << end - begin << std::endl;
	return 0;
}
