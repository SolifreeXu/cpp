#include "Numeric.hpp"

#include <type_traits>
#include <iomanip>
#include <iostream>

int main()
{
	const char* stringList[] = { "", "Eterfree", " + ", " - ", " ++ ", " -- ", " +1-1 ", " -1+1 ", " +0 ", " -0 ", " 123 ", " +123 ", " -123 " };
	constexpr auto size = sizeof stringList / sizeof stringList[0];
	for (std::remove_const_t<decltype(size)> index = 0; index < size; ++index)
	{
		using std::cout, std::endl, std::left, std::boolalpha;
		using std::setw;

		auto string = stringList[index];
		cout << left << boolalpha;
		cout << setw(10) << "string: " << string << std::endl;
		cout << setw(10) << "all: " << valid(string, IntegerType::ALL) << endl;
		cout << setw(10) << "natural: " << valid(string, IntegerType::NATURAL) << endl;
		cout << setw(10) << "positive: " << valid(string, IntegerType::POSITIVE) << endl;
		cout << setw(10) << "negative: " << valid(string, IntegerType::NEGATIVE) << endl;
		cout << endl;
	}
	return 0;
}
