#include "algorithm.h"

#include <iostream>
#include <ctime>

int main()
{
	int x = 9, y = 10;
	std::clock_t begin = std::clock();
	for (int i = 0; i < 1000000; ++i)
		forever::swap(x, y);
	std::clock_t end = std::clock();
	std::cout << x << ' ' << y << std::endl;
	std::cout << end - begin;
	return 0;
}