#include <iostream>
#include "Complex.h"

int main()
{
	Complex a(1, 2), b(0, 1);
	Complex c;
	std::cout << "请输入一个复数：";
	std::cin >> c;
	std::cout << c - a << std::endl;
	std::cout << (c /= b) << std::endl;
	std::cout << a + b << std::endl;
	std::cout << b + 9.7 << std::endl;
	std::cout << 9.7 + b << std::endl;
	return 0;
}