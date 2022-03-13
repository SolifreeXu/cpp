#include "BitSet.hpp"

#include <concepts>
#include <cstdint>
#include <iostream>

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

	cout << "set " << low << endl;
	bitSet.set(low);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << "reset " << low << " and flip " << middle << endl;
	bitSet.reset(low);
	bitSet.flip(middle);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << "set [" << low << ", " << middle << ')' << endl;
	bitSet.set(low, middle);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << "reset [" << low << ", " << middle << ')' << endl;
	bitSet.reset(low, middle);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << "flip [" << middle << ", " << high << ')' << endl;
	bitSet.flip(middle, high);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	auto offset = middle >> 1;

	auto from = middle - offset, to = high - offset;
	cout << "set [" << from << ", " << to << ')' << endl;
	bitSet.set(from, to);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << "reset [" << from << ", " << to << ')' << endl;
	bitSet.reset(from, to);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	cout << "flip [" << from << ", " << to << ')' << endl;
	bitSet.flip(from, to);
	print(bitSet);
	print(bitSet, low, middle);
	cout << endl;

	from = low;
	to = middle + offset;
	cout << "copy [" << from << ", " << to << ')' << endl;
	auto bitSet3 = bitSet.copy(from, to);
	print(bitSet3);
	print(bitSet3, low, middle);
	cout << endl;

	from = middle + offset;
	to = high + offset;
	cout << "reset [" << from << ", " << to << ')' << endl;
	bitSet.reset(from, to);
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	cout << "set [" << from << ", " << to << ')' << endl;
	bitSet.set(from, to);
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	cout << "flip [" << from << ", " << to << ')' << endl;
	bitSet.flip(from, to);
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	from = middle;
	to = high + middle + offset;
	cout << "copy [" << from << ", " << to << ')' << endl;
	bitSet3 = bitSet.copy(from, to);
	print(bitSet3);
	print(bitSet3, low, middle, high);
	cout << endl;

	cout << "flip all" << endl;
	bitSet.flip();
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	cout << "reset all" << endl;
	bitSet.reset();
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	cout << "set all" << endl;
	bitSet.set();
	print(bitSet);
	print(bitSet, low, middle, high);
	cout << endl;

	cout << "operator<< " << offset << endl;
	auto bitSet1 = bitSet << offset;
	print(bitSet1);
	print(bitSet1, low, middle, high);
	cout << endl;

	cout << "operator>> " << offset << endl;
	auto bitSet2 = bitSet >> offset;
	print(bitSet2);
	print(bitSet2, low, middle, high);
	cout << endl;

	offset = middle;

	cout << "operator<< " << offset << endl;
	bitSet1 = bitSet << offset;
	print(bitSet1);
	print(bitSet1, low, middle, high);
	cout << endl;

	cout << "operator>> " << offset << endl;
	bitSet2 = bitSet >> offset;
	print(bitSet2);
	print(bitSet2, low, middle, high);
	cout << endl;

	cout << "operator&" << endl;
	bitSet3 = bitSet1 & bitSet2;
	print(bitSet3);
	print(bitSet3, low, middle, high);
	cout << endl;

	cout << "operator|" << endl;
	bitSet3 = bitSet1 | bitSet2;
	print(bitSet3);
	print(bitSet3, low, middle, high);
	cout << endl;

	cout << "operator^" << endl;
	bitSet3 = bitSet1 ^ bitSet2;
	print(bitSet3);
	print(bitSet3, low, middle, high);
	cout << endl;

	cout << "operator~" << endl;
	bitSet3 = ~bitSet;
	print(bitSet3);
	print(bitSet3, low, middle, high);
	cout << endl;
	return 0;
}
