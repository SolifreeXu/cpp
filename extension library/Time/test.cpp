#include <iostream>
#include "Time.h"

int main()
{
	Time t1;
	std::cout << "the time is now ";
	std::cout << t1 << std::endl;
	Time t2(19, 27, 30);
	std::cout << "the custom time is ";
	std::cout << t2 << std::endl;
	Time t3 = t1 + t2;
	std::cout << "the sum of time is ";
	std::cout << t3;
	return 0;
}