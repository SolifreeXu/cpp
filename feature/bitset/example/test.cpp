#include "BitSet.hpp"

#include <concepts>
#include <cstdlib>
#include <climits>
#include <cstdint>
#include <iostream>

#define STRING(content) #content

template <std::unsigned_integral _ValueType>
static void print(const BitSet<_ValueType>& _bitSet)
{
	using std::cout, std::endl;

	cout << std::boolalpha;
	cout << "size: " << _bitSet.size() << endl;
	cout << "count: " << _bitSet.count() << endl;
	cout << "all: " << _bitSet.all() << endl;
	cout << "any: " << _bitSet.any() << endl;
	cout << "none: " << _bitSet.none() << endl;
}

template <std::unsigned_integral _ValueType>
static void print(const BitSet<_ValueType>& _bitSet, \
	typename BitSet<_ValueType>::SizeType _position)
{
	std::cout << std::boolalpha << _position << ": " \
		<< _bitSet[_position] << std::endl;
}

template <std::unsigned_integral _ValueType, \
	typename... _Positions>
static void print(const BitSet<_ValueType>& _bitSet, \
	typename BitSet<_ValueType>::SizeType _position, \
	_Positions... _positions)
{
	print(_bitSet, _position);
	print(_bitSet, _positions...);
}

int main()
{
	using BitSet = BitSet<std::uint64_t>;

	using std::cout, std::endl;

	constexpr auto SIZE = sizeof(BitSet::ValueType) * CHAR_BIT;
	decltype(SIZE) LOW = 0;
	decltype(SIZE) MIDDLE = LOW + SIZE;
	decltype(SIZE) HIGH = MIDDLE + SIZE;

	BitSet bitSet(2);

	cout << STRING(bitSet) << " set " << LOW << endl;
	bitSet.set(LOW);
	print(bitSet);
	print(bitSet, LOW, MIDDLE);
	cout << endl;

	cout << STRING(bitSet) << " reset " << LOW \
		<< " and flip " << MIDDLE << endl;
	bitSet.reset(LOW);
	bitSet.flip(MIDDLE);
	print(bitSet);
	print(bitSet, LOW, MIDDLE);
	cout << endl;

	auto from = LOW, to = MIDDLE;

	cout << STRING(bitSet) \
		<< " set [" << from << ", " << to << ')' << endl;
	bitSet.set(from, to);
	print(bitSet);
	print(bitSet, LOW, MIDDLE);
	cout << endl;

	cout << STRING(bitSet) \
		<< " reset [" << from << ", " << to << ')' << endl;
	bitSet.reset(from, to);
	print(bitSet);
	print(bitSet, LOW, MIDDLE);
	cout << endl;

	from = MIDDLE;
	to = HIGH;

	cout << STRING(bitSet) \
		<< " flip [" << from << ", " << to << ')' << endl;
	bitSet.flip(from, to);
	print(bitSet);
	print(bitSet, LOW, MIDDLE);
	cout << endl;

	auto offset = MIDDLE >> 1;
	from = MIDDLE - offset;
	to = HIGH - offset;

	cout << STRING(bitSet) \
		<< " set [" << from << ", " << to << ')' << endl;
	bitSet.set(from, to);
	print(bitSet);
	print(bitSet, LOW, MIDDLE);
	cout << endl;

	cout << STRING(bitSet) \
		<< " reset [" << from << ", " << to << ')' << endl;
	bitSet.reset(from, to);
	print(bitSet);
	print(bitSet, LOW, MIDDLE);
	cout << endl;

	cout << STRING(bitSet) \
		<< " flip [" << from << ", " << to << ')' << endl;
	bitSet.flip(from, to);
	print(bitSet);
	print(bitSet, LOW, MIDDLE);
	cout << endl;

	from = LOW;
	to = MIDDLE + offset;

	auto bitSet2 = bitSet.copy(from, to);

	cout << STRING(bitSet2) << " from " << STRING(bitSet) \
		<< " copy [" << from << ", " << to << ')' << endl;
	print(bitSet2);
	print(bitSet2, LOW, MIDDLE);
	cout << endl;

	from = MIDDLE + offset;
	to = HIGH + offset;

	cout << STRING(bitSet) \
		<< " reset [" << from << ", " << to << ')' << endl;
	bitSet.reset(from, to);
	print(bitSet);
	print(bitSet, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSet) \
		<< " set [" << from << ", " << to << ')' << endl;
	bitSet.set(from, to);
	print(bitSet);
	print(bitSet, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSet) \
		<< " flip [" << from << ", " << to << ')' << endl;
	bitSet.flip(from, to);
	print(bitSet);
	print(bitSet, LOW, MIDDLE, HIGH);
	cout << endl;

	from = MIDDLE;
	to = HIGH + MIDDLE + offset;

	cout << STRING(bitSet2) << " from " << STRING(bitSet) \
		<< " copy [" << from << ", " << to << ')' << endl;
	bitSet2 = bitSet.copy(from, to);
	print(bitSet2);
	print(bitSet2, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSet) << " flip all" << endl;
	bitSet.flip();
	print(bitSet);
	print(bitSet, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSet) << " reset all" << endl;
	bitSet.reset();
	print(bitSet);
	print(bitSet, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSet) << " set all" << endl;
	bitSet.set();
	print(bitSet);
	print(bitSet, LOW, MIDDLE, HIGH);
	cout << endl;

	auto bitSetA = bitSet << offset;

	cout << STRING(bitSetA) << " = " \
		<< STRING(bitSet) << " << " << offset << endl;
	print(bitSetA);
	print(bitSetA, LOW, MIDDLE, HIGH);
	cout << endl;

	auto bitSetB = bitSet >> offset;

	cout << STRING(bitSetB) << " = " \
		<< STRING(bitSet) << " >> " << offset << endl;
	print(bitSetB);
	print(bitSetB, LOW, MIDDLE, HIGH);
	cout << endl;

	offset = MIDDLE;

	cout << STRING(bitSetA) << " = " \
		<< STRING(bitSet) << " << " << offset << endl;
	bitSetA = bitSet << offset;
	print(bitSetA);
	print(bitSetA, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSetB) << " = " \
		<< STRING(bitSet) << " >> " << offset << endl;
	bitSetB = bitSet >> offset;
	print(bitSetB);
	print(bitSetB, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSet2) << " = " \
		<< STRING(bitSetA) << " & " << STRING(bitSetB) << endl;
	bitSet2 = bitSetA & bitSetB;
	print(bitSet2);
	print(bitSet2, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSet2) << " = " \
		<< STRING(bitSetA) << " | " << STRING(bitSetB) << endl;
	bitSet2 = bitSetA | bitSetB;
	print(bitSet2);
	print(bitSet2, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSet2) << " = " \
		<< STRING(bitSetA) << " ^ " << STRING(bitSetB) << endl;
	bitSet2 = bitSetA ^ bitSetB;
	print(bitSet2);
	print(bitSet2, LOW, MIDDLE, HIGH);
	cout << endl;

	cout << STRING(bitSet2) << " = ~" << STRING(bitSet) << endl;
	bitSet2 = ~bitSet;
	print(bitSet2);
	print(bitSet2, LOW, MIDDLE, HIGH);
	cout << endl;
	return EXIT_SUCCESS;
}
