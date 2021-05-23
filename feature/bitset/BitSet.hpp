#pragma once

#include <climits>
#include <string>
#include <cmath>

template <typename _SizeType = size_t>
class BitSet
{
public:
	using SizeType = _SizeType;

private:
	static const SizeType _charBit;
	std::string _byteSet;

private:
	static SizeType size(SizeType _capacity) noexcept
	{
		return (_capacity >> _charBit) + ((_capacity & CHAR_BIT - 1) > 0);
	}

	static char generate(SizeType _position) noexcept
	{
		return '\1' << (_position & CHAR_BIT - 1);
	}

	void reserve(SizeType _position)
	{
		auto size = BitSet::size(_position);
		if (size > _byteSet.size())
			_byteSet.resize(size, '\0');
	}

public:
	BitSet(SizeType _capacity)
		: _byteSet(size(_capacity), '\0') {}

	BitSet(const char* _data, SizeType _size)
		: _byteSet(_data, _size) {}

	bool operator==(const BitSet& _another) const noexcept
	{
		return _byteSet == _another._byteSet;
	}

	bool operator!=(const BitSet& _another) const noexcept
	{
		return _byteSet != _another._byteSet;
	}

	bool operator[](SizeType _position) const noexcept
	{
		if (size(_position) > _byteSet.size())
			return false;

		return (_byteSet[_position >> _charBit] & generate(_position)) > 0;
	}

	BitSet operator~() const noexcept
	{
		return BitSet(*this).flip();
	}

	// 获取字节内容
	const char* data() const noexcept
	{
		return _byteSet.data();
	}

	// 获取字节数
	SizeType size() const noexcept
	{
		return _byteSet.size();
	}

	// 获取位数
	SizeType count() const noexcept;

	// 所有位为真
	bool all() const noexcept;

	// 任意位为真
	bool any() const noexcept;

	// 无位为真
	bool none() const noexcept
	{
		return !any();
	}

	// 更改字节数
	void resize(SizeType size)
	{
		_byteSet.resize(size, '\0');
	}

	// 设置所有位
	BitSet& set()
	{
		_byteSet.replace(0, _byteSet.size(), _byteSet.size(), ~'\0');
		return *this;
	}

	// 设置指定位
	BitSet& set(SizeType _position, bool _value = true)
	{
		if (!_value)
			return reset(_position);

		reserve(_position);

		_byteSet[_position >> _charBit] |= generate(_position);
		return *this;
	}

	// 重置所有位
	BitSet& reset()
	{
		_byteSet.replace(0, _byteSet.size(), _byteSet.size(), '\0');
		return *this;
	}

	// 重置指定位
	BitSet& reset(SizeType _position)
	{
		reserve(_position);

		_byteSet[_position >> _charBit] &= ~generate(_position);
		return *this;
	}

	// 翻转所有位
	BitSet& flip() noexcept;

	// 翻转指定位
	BitSet& flip(SizeType _position)
	{
		reserve(_position);

		_byteSet[_position >> _charBit] ^= generate(_position);
		return *this;
	}
};

template <typename _SizeType>
const typename BitSet<_SizeType>::SizeType BitSet<_SizeType>::_charBit = static_cast<SizeType>(std::log2(CHAR_BIT));

// 获取位数
template <typename _SizeType>
typename BitSet<_SizeType>::SizeType BitSet<_SizeType>::count() const noexcept
{
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
	for (decltype(_byteSet.size()) index = 0; index < _byteSet.size(); ++index)
		counter += bitTable[static_cast<unsigned char>(_byteSet[index])];
	return counter;
}

// 所有位为真
template <typename _SizeType>
bool BitSet<_SizeType>::all() const noexcept
{
	for (decltype(_byteSet.size()) index = 0; index < _byteSet.size(); ++index)
		if (_byteSet[index] != ~'\0')
			return false;
	return true;
}

// 任意位为真
template <typename _SizeType>
bool BitSet<_SizeType>::any() const noexcept
{
	for (decltype(_byteSet.size()) index = 0; index < _byteSet.size(); ++index)
		if (_byteSet[index] != '\0')
			return true;
	return false;
}

// 翻转所有位
template <typename _SizeType>
BitSet<_SizeType>& BitSet<_SizeType>::flip() noexcept
{
	for (decltype(_byteSet.size()) index = 0; index < _byteSet.size(); ++index)
		_byteSet[index] = ~_byteSet[index];
	return *this;
}
