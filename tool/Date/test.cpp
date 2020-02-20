#include <iostream>
#include "Date.h"

int main()
{
	Date date;
	std::cout << date << std::endl;
	date.setYear(-100);
	date.setMonth(2);
	date.setDay(29);
	std::cout << date << std::endl;
	return 0;
}