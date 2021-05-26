#include "BitSet.hpp"

#include <iostream>

template <typename _ValueType, typename _SizeType>
void dump(const BitSet<_ValueType, _SizeType>& bitSet, _SizeType x1, _SizeType x2)
{
	using std::cout;
	using std::boolalpha;
	using std::endl;

	cout << boolalpha;
	cout << "x1: " << bitSet[x1] << endl;
	cout << "x2: " << bitSet[x2] << endl;
	cout << "count: " << bitSet.count() << endl;
	cout << "all: " << bitSet.all() << endl;
	cout << "any: " << bitSet.any() << endl;
	cout << "none: " << bitSet.none() << endl;
}

int main()
{
	using std::cout;
	using std::endl;

	BitSet<uint64_t> bitSet(2);
	BitSet<uint64_t>::SizeType x1 = 9, x2 = 79;

	bitSet.set(x1);
	dump(bitSet, x1, x2);
	cout << endl;

	bitSet.reset(x1);
	bitSet.flip(x2);
	dump(bitSet, x1, x2);
	cout << endl;

	bitSet.flip();
	dump(bitSet, x1, x2);
	cout << endl;

	bitSet.set();
	dump(bitSet, x1, x2);
	cout << endl;

	bitSet.reset();
	dump(bitSet, x1, x2);
	return 0;
}
