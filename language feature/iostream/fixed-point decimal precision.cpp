#include <cmath>
#include <iostream>

int main()
{
	const double PI = atan(1.0) * 4; // 反三角函数arctan
	using std::cout;
	cout.setf(std::ios::fixed);
	cout.precision(7);
	cout << PI << std::endl;
	return 0;
}
