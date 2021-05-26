#pragma once

#include <cstddef>
#include <climits>
#include <vector>
#include <cmath>

template <typename _ValueType, typename _SizeType = std::size_t>
class BitSet
{
public:
	using ValueType = _ValueType;
	using SizeType = _SizeType;

private:
	static constexpr SizeType _elementSize = CHAR_BIT * sizeof(ValueType) - 1;
	static const SizeType _sizeBit;
	std::vector<ValueType> _elementSet;

private:
	static SizeType size(SizeType _capacity) noexcept
	{
		return (_capacity >> _sizeBit) + ((_capacity & _elementSize) > 0);
	}

	static ValueType generate(SizeType _position) noexcept
	{
		return static_cast<ValueType>(1) << (_position & _elementSize);
	}

	void reserve(SizeType _capacity)
	{
		auto size = BitSet::size(_capacity);
		if (size > _elementSet.size())
			_elementSet.resize(size, 0);
	}

public:
	BitSet(SizeType _size)
		: _elementSet(_size, 0) {}

	BitSet(const ValueType* _data, SizeType _size)
		: _elementSet(_data, _data + _size) {}

	bool operator==(const BitSet& _another) const
	{
		return _elementSet == _another._elementSet;
	}

	bool operator!=(const BitSet& _another) const
	{
		return _elementSet != _another._elementSet;
	}

	bool operator[](SizeType _position) const noexcept
	{
		return size(_position) <= _elementSet.size() && (_elementSet[_position >> _sizeBit] & generate(_position)) > 0;
	}

	BitSet& operator&=(const BitSet& _another) noexcept;

	BitSet& operator|=(const BitSet& _another) noexcept;

	BitSet& operator^=(const BitSet& _another) noexcept;

	BitSet operator~() const noexcept
	{
		return BitSet(*this).flip();
	}

	// 获取元素内容
	const ValueType* data() const noexcept
	{
		return _elementSet.data();
	}

	// 获取元素数量
	SizeType size() const noexcept
	{
		return _elementSet.size();
	}

	// 统计有效位
	SizeType count() const noexcept;

	// 所有位有效
	bool all() const noexcept;

	// 任意位有效
	bool any() const noexcept;

	// 无位有效
	bool none() const noexcept
	{
		return !any();
	}

	// 改变元素数量
	void resize(SizeType _size)
	{
		_elementSet.resize(_size, 0);
	}

	// 设置所有位
	BitSet& set()
	{
		_elementSet.assign(_elementSet.size(), ~static_cast<ValueType>(0));
		return *this;
	}

	// 设置指定位
	BitSet& set(SizeType _position, bool _value = true)
	{
		if (!_value)
			return reset(_position);

		reserve(_position);

		_elementSet[_position >> _sizeBit] |= generate(_position);
		return *this;
	}

	// 重置所有位
	BitSet& reset()
	{
		_elementSet.assign(_elementSet.size(), 0);
		return *this;
	}

	// 重置指定位
	BitSet& reset(SizeType _position)
	{
		reserve(_position);

		_elementSet[_position >> _sizeBit] &= ~generate(_position);
		return *this;
	}

	// 翻转所有位
	BitSet& flip() noexcept;

	// 翻转指定位
	BitSet& flip(SizeType _position)
	{
		reserve(_position);

		_elementSet[_position >> _sizeBit] ^= generate(_position);
		return *this;
	}
};

template <typename _ValueType, typename _SizeType>
const typename BitSet<_ValueType, _SizeType>::SizeType BitSet<_ValueType, _SizeType>::_sizeBit = static_cast<SizeType>(std::log2(CHAR_BIT * sizeof ValueType));

template <typename _ValueType, typename _SizeType>
BitSet<_ValueType, _SizeType>& BitSet<_ValueType, _SizeType>::operator&=(const BitSet& _another) noexcept
{
	if (_elementSet.size() == _another._elementSet.size())
		for (decltype(_elementSet.size()) index = 0; index < _elementSet.size(); ++index)
			_elementSet[index] &= _another._elementSet[index];
	return *this;
}

template <typename _ValueType, typename _SizeType>
BitSet<_ValueType, _SizeType>& BitSet<_ValueType, _SizeType>::operator|=(const BitSet& _another) noexcept
{
	if (_elementSet.size() == _another._elementSet.size())
		for (decltype(_elementSet.size()) index = 0; index < _elementSet.size(); ++index)
			_elementSet[index] |= _another._elementSet[index];
	return *this;
}

template <typename _ValueType, typename _SizeType>
BitSet<_ValueType, _SizeType>& BitSet<_ValueType, _SizeType>::operator^=(const BitSet& _another) noexcept
{
	if (_elementSet.size() == _another._elementSet.size())
		for (decltype(_elementSet.size()) index = 0; index < _elementSet.size(); ++index)
			_elementSet[index] ^= _another._elementSet[index];
	return *this;
}

// 统计有效位
template <typename _ValueType, typename _SizeType>
typename BitSet<_ValueType, _SizeType>::SizeType BitSet<_ValueType, _SizeType>::count() const noexcept
{
	static constexpr unsigned char maxChar = ~static_cast<unsigned char>('\0');

	const char* const bitTable = \
		"\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4"
		"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
		"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
		"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
		"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
		"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
		"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
		"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
		"\1\2\2\3\2\3\3\4\2\3\3\4\3\4\4\5"
		"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
		"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
		"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
		"\2\3\3\4\3\4\4\5\3\4\4\5\4\5\5\6"
		"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
		"\3\4\4\5\4\5\5\6\4\5\5\6\5\6\6\7"
		"\4\5\5\6\5\6\6\7\5\6\6\7\6\7\7\x8";

	SizeType counter = 0;
	for (decltype(_elementSet.size()) index = 0; index < _elementSet.size(); ++index)
	{
		auto element = _elementSet[index];
		for (decltype(sizeof element) index = 0; index < sizeof element; ++index)
		{
			counter += bitTable[element & maxChar];
			element >>= CHAR_BIT;
		}
	}
	return counter;
}

// 所有位为真
template <typename _ValueType, typename _SizeType>
bool BitSet<_ValueType, _SizeType>::all() const noexcept
{
	static constexpr ValueType maxElement = ~static_cast<ValueType>(0);
	for (decltype(_elementSet.size()) index = 0; index < _elementSet.size(); ++index)
		if (_elementSet[index] != maxElement)
			return false;
	return true;
}

// 任意位为真
template <typename _ValueType, typename _SizeType>
bool BitSet<_ValueType, _SizeType>::any() const noexcept
{
	for (decltype(_elementSet.size()) index = 0; index < _elementSet.size(); ++index)
		if (_elementSet[index] != 0)
			return true;
	return false;
}

// 翻转所有位
template <typename _ValueType, typename _SizeType>
BitSet<_ValueType, _SizeType>& BitSet<_ValueType, _SizeType>::flip() noexcept
{
	for (decltype(_elementSet.size()) index = 0; index < _elementSet.size(); ++index)
		_elementSet[index] = ~_elementSet[index];
	return *this;
}
