#include "BitSet.hpp"

#include <concepts>
#include <cstdint>
#include <iostream>

#define STRING(content) #content

template <std::unsigned_integral _ValueType>
void print(const BitSet<_ValueType>& _bitSet)
{
	using std::cout;
	using std::endl;

	cout << std::boolalpha;
	cout << "size: " << _bitSet.size() << endl;
	cout << "count: " << _bitSet.count() << endl;
	cout << "all: " << _bitSet.all() << endl;
	cout << "any: " << _bitSet.any() << endl;
	cout << "none: " << _bitSet.none() << endl;
}

template <std::unsigned_integral _ValueType>
void print(const BitSet<_ValueType>& _bitSet, typename BitSet<_ValueType>::SizeType _position)
{
	std::cout << std::boolalpha << _position << ": " << _bitSet[_position] << std::endl;
}

template <std::unsigned_integral _ValueType, typename... _Positions>
void print(const BitSet<_ValueType>& _bitSet, typename BitSet<_ValueType>::SizeType _position, _Positions... _positions)
{
	print(_bitSet, _position);
	print(_bitSet, _positions...);
}

int main()
{
	using std::cout;
	using std::endl;

	using BitSetType = BitSet<uint64_t>;

	constexpr BitSetType::SizeType low = 0, middle = 64, high = 128;

	BitSetType bitSet(2);

	cout << STRING(bitSet) << " set " << low << endl;
	bitSet.set(low);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << STRING(bitSet) << " reset " << low << " and flip " << middle << endl;
	bitSet.reset(low);
	bitSet.flip(middle);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << STRING(bitSet) << " set [" << low << ", " << middle << ')' << endl;
	bitSet.set(low, middle);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << STRING(bitSet) << " reset [" << low << ", " << middle << ')' << endl;
	bitSet.reset(low, middle);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << STRING(bitSet) << " flip [" << middle << ", " << high << ')' << endl;
	bitSet.flip(middle, high);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	auto offset = middle >> 1;

	auto from = middle - offset, to = high - offset;
	cout << STRING(bitSet) << " set [" << from << ", " << to << ')' << endl;
	bitSet.set(from, to);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << STRING(bitSet) << " reset [" << from << ", " << to << ')' << endl;
	bitSet.reset(from, to);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << STRING(bitSet) << " flip [" << from << ", " << to << ')' << endl;
	bitSet.flip(from, to);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	from = low;
	to = middle + offset;
	auto bitSet2 = bitSet.copy(from, to);
	cout << STRING(bitSet2) << " from " << STRING(bitSet) << " copy [" << from << ", " << to << ')' << endl;
	print(bitSet2);
	print(bitSet2, low, middle);
	cout << endl;

	from = middle + offset;
	to = high + offset;
	cout << STRING(bitSet) << " reset [" << from << ", " << to << ')' << endl;
	bitSet.reset(from, to);
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	cout << STRING(bitSet) << " set [" << from << ", " << to << ')' << endl;
	bitSet.set(from, to);
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	cout << STRING(bitSet) << " flip [" << from << ", " << to << ')' << endl;
	bitSet.flip(from, to);
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	from = middle;
	to = high + middle + offset;
	cout << STRING(bitSet2) << " from " << STRING(bitSet) << " copy [" << from << ", " << to << ')' << endl;
	bitSet2 = bitSet.copy(from, to);
	print(bitSet2);
	print(bitSet2, low, middle, high);
	cout << endl;

	cout << STRING(bitSet) << " flip all" << endl;
	bitSet.flip();
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	cout << STRING(bitSet) << " reset all" << endl;
	bitSet.reset();
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	cout << STRING(bitSet) << " set all" << endl;
	bitSet.set();
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	auto bitSetA = bitSet << offset;
	cout << STRING(bitSetA) << " = " << STRING(bitSet) << " << " << offset << endl;
	print(bitSetA);
	print(bitSetA, low, middle, high);
	cout << endl;

	auto bitSetB = bitSet >> offset;
	cout << STRING(bitSetB) << " = " << STRING(bitSet) << " >> " << offset << endl;
	print(bitSetB);
	print(bitSetB, low, middle, high);
	cout << endl;

	offset = middle;

	cout << STRING(bitSetA) << " = " << STRING(bitSet) << " << " << offset << endl;
	bitSetA = bitSet << offset;
	print(bitSetA);
	print(bitSetA, low, middle, high);
	cout << endl;

	cout << STRING(bitSetB) << " = " << STRING(bitSet) << " >> " << offset << endl;
	bitSetB = bitSet >> offset;
	print(bitSetB);
	print(bitSetB, low, middle, high);
	cout << endl;

	cout << STRING(bitSet2) << " = " << STRING(bitSetA) << " & " << STRING(bitSetB) << endl;
	bitSet2 = bitSetA & bitSetB;
	print(bitSet2);
	print(bitSet2, low, middle, high);
	cout << endl;

	cout << STRING(bitSet2) << " = " << STRING(bitSetA) << " | " << STRING(bitSetB) << endl;
	bitSet2 = bitSetA | bitSetB;
	print(bitSet2);
	print(bitSet2, low, middle, high);
	cout << endl;

	cout << STRING(bitSet2) << " = " << STRING(bitSetA) << " ^ " << STRING(bitSetB) << endl;
	bitSet2 = bitSetA ^ bitSetB;
	print(bitSet2);
	print(bitSet2, low, middle, high);
	cout << endl;

	cout << STRING(bitSet2) << " = ~" << STRING(bitSet) << endl;
	bitSet2 = ~bitSet;
	print(bitSet2);
	print(bitSet2, low, middle, high);
	cout << endl;
	return 0;
}
