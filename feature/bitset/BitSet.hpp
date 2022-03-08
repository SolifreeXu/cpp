﻿/*
* 文件名称：BitSet.hpp
* 语言标准：C++20
* 
* 创建日期：2021年05月24日
* 更新日期：2022年03月09日
* 
* 摘要：
* 1.位集合类模板参照C++标准，自定义无符号整型元素，节省内存而不受字节序影响。
* 2.位集合支持动态扩大容量，提供位与集合两种位运算。
* 
* 用例：
* 1.位集合如何移除另一位集合含有的元素？
*   二者依次进行或运算和异或运算。
* 2.位集合如何实现旋转？
*   分别左移与右移，得到两个中间位集合，二者进行或运算。
* 
* 作者：许聪
* 邮箱：solifree@qq.com
* 
* 版本：v1.0.1
* 变化
* v1.0.1
* 1.新增范围设置、重置、翻转、复制，以及位移运算。
*/

#pragma once

#include <concepts>
#include <type_traits>
#include <functional>
#include <climits>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>

template <std::unsigned_integral _ValueType>
class BitSet
{
	using VectorType = std::vector<_ValueType>;
	using FunctorType = std::function<void(_ValueType&, _ValueType)>;

public:
	using ValueType = _ValueType;
	using SizeType = VectorType::size_type;

private:
	static constexpr ValueType _maxElement = ~static_cast<ValueType>(0); // 单元素最大值
	static constexpr SizeType _maxPosition = CHAR_BIT * sizeof(ValueType) - 1; // 单元素之位置最大值
	static const SizeType _bitSize; // 单元素之位数量
	VectorType _vector; // 元素向量

private:
	// 位容量
	static auto capacity(SizeType _position) noexcept
	{
		return _position + 1;
	}

	// 元素数量
	static auto size(SizeType _capacity) noexcept
	{
		return (_capacity >> _bitSize) + ((_capacity & _maxPosition) > 0);
	}

	// 生成单元素
	static auto generate(SizeType _position) noexcept
	{
		return static_cast<ValueType>(1) << (_position & _maxPosition);
	}

	// 预留空间
	void reserve(SizeType _capacity)
	{
		if (auto size = BitSet::size(_capacity); size > _vector.size())
			_vector.resize(size, 0);
	}

	// 判断所有元素值
	bool all(const ValueType& _element) const noexcept;

	// 遍历指定范围元素
	void traverse(SizeType _begin, SizeType _end, FunctorType _functor) noexcept;

public:
	BitSet(SizeType _size = 0)
		: _vector(_size, 0) {}

	BitSet(const ValueType* _data, SizeType _size)
		: _vector(_data, _data + _size) {}

	bool operator[](SizeType _position) const noexcept
	{
		return size(capacity(_position)) <= _vector.size() \
			&& (_vector[_position >> _bitSize] & generate(_position)) > 0;
	}

	BitSet& operator&=(const BitSet& _bitSet);

	BitSet& operator|=(const BitSet& _bitSet);

	BitSet& operator^=(const BitSet& _bitSet);

	auto operator~() const
	{
		return BitSet(*this).flip();
	}

	BitSet& operator<<=(SizeType _position) noexcept;

	BitSet& operator>>=(SizeType _position) noexcept;

	BitSet operator<<(SizeType _position) const
	{
		return BitSet(*this) <<= _position;
	}

	BitSet operator>>(SizeType _position) const
	{
		return BitSet(*this) >>= _position;
	}

	friend bool operator==(const BitSet& _left, const BitSet& _right) noexcept
	{
		return _left._vector == _right._vector;
	}

	// C++ 20 != 采用 == 推导
	//friend bool operator!=(const BitSet& _left, const BitSet& _right) noexcept
	//{
	//	return _left._vector != _right._vector;
	//}

	friend BitSet operator&(const BitSet& _left, const BitSet& _right)
	{
		return BitSet(_left) &= _right;
	}

	friend BitSet operator|(const BitSet& _left, const BitSet& _right)
	{
		return BitSet(_left) |= _right;
	}

	friend BitSet operator^(const BitSet& _left, const BitSet& _right)
	{
		return BitSet(_left) ^= _right;
	}

	// 获取元素内容
	auto data() const noexcept
	{
		return _vector.data();
	}

	// 获取元素数量
	auto size() const noexcept
	{
		return _vector.size();
	}

	// 统计有效位
	SizeType count() const noexcept;

	// 所有位有效
	bool all() const noexcept
	{
		return all(_maxElement);
	}

	// 任意位有效
	bool any() const noexcept
	{
		return !all(0);
	}

	// 无有效位
	bool none() const noexcept
	{
		return all(0);
	}

	// 改变元素数量
	BitSet& resize(SizeType _size, bool _forced = false);

	// 设置指定位
	BitSet& set(SizeType _position, bool _value = true);

	// 设置指定范围
	BitSet& set(SizeType _begin, SizeType _end, bool _value = true);

	// 设置所有位
	BitSet& set() noexcept
	{
		_vector.assign(_vector.size(), _maxElement);
		return *this;
	}

	// 重置指定位
	BitSet& reset(SizeType _position) noexcept;

	// 重置指定范围
	BitSet& reset(SizeType _begin, SizeType _end) noexcept;

	// 重置所有位
	BitSet& reset() noexcept
	{
		_vector.assign(_vector.size(), 0);
		return *this;
	}

	// 翻转指定位
	BitSet& flip(SizeType _position);

	// 翻转指定范围
	BitSet& flip(SizeType _begin, SizeType _end);

	// 翻转所有位
	BitSet& flip() noexcept;

	// 复制指定范围
	BitSet copy(SizeType _begin, SizeType _end) const;
};

template <std::unsigned_integral _ValueType>
const typename BitSet<_ValueType>::SizeType BitSet<_ValueType>::_bitSize = static_cast<SizeType>(std::log2(CHAR_BIT * sizeof(ValueType)));

// 判断所有元素值
template <std::unsigned_integral _ValueType>
bool BitSet<_ValueType>::all(const ValueType& _element) const noexcept
{
	for (const auto& element : _vector)
		if (element != _element)
			return false;
	return true;
}

// 遍历指定范围元素
template <std::unsigned_integral _ValueType>
void BitSet<_ValueType>::traverse(SizeType _begin, SizeType _end, FunctorType _functor) noexcept
{
	if (!_functor)
		return;

	constexpr auto maxPosition = _maxPosition + 1;
	decltype(_begin) beginPosition = _begin & _maxPosition;
	decltype(_end) endPosition = _end & _maxPosition;

	using ValueType = std::remove_const_t<decltype(_maxElement)>;
	ValueType beginMask = _maxElement << beginPosition;
	decltype(beginMask) endMask = ~(_maxElement << endPosition);

	decltype(_begin) begin = _begin >> _bitSize;
	decltype(_end) end = _end >> _bitSize;
	auto size = std::min(_vector.size(), end + ((_end & _maxPosition) > 0));
	for (auto index = begin; index < size; ++index)
	{
		auto mask = _maxElement;
		if (index == begin)
			mask &= beginMask;

		if (index == end)
			mask &= endMask;

		_functor(_vector[index], mask);
	}
}

template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::operator&=(const BitSet& _bitSet)
{
	auto size = std::max(_vector.size(), _bitSet._vector.size());
	if (size <= 0)
		return *this;

	resize(size);

	decltype(size) index = 0;
	for (; index < _bitSet._vector.size(); ++index)
		_vector[index] &= _bitSet._vector[index];

	if (index < size)
		std::memset(_vector.data() + index, 0, sizeof(ValueType) * (size - index));
	return *this;
}

template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::operator|=(const BitSet& _bitSet)
{
	auto size = std::max(_vector.size(), _bitSet._vector.size());
	if (size <= 0)
		return *this;

	resize(size);

	for (decltype(size) index = 0; \
		index < _bitSet._vector.size(); ++index)
		_vector[index] |= _bitSet._vector[index];
	return *this;
}

template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::operator^=(const BitSet& _bitSet)
{
	auto size = std::max(_vector.size(), _bitSet._vector.size());
	if (size <= 0)
		return *this;

	resize(size);

	for (decltype(size) index = 0; \
		index < _bitSet._vector.size(); ++index)
		_vector[index] ^= _bitSet._vector[index];
	return *this;
}

template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::operator<<=(SizeType _position) noexcept
{
	constexpr auto maxPosition = _maxPosition + 1;
	decltype(_position) position = _position & _maxPosition;

	using ValueType = std::remove_const_t<decltype(_maxElement)>;
	ValueType lowMask = _maxElement >> position;
	decltype(lowMask) highMask = ~lowMask;

	auto size = _vector.size();
	decltype(size) index = size;
	for (decltype(index) offset = _position >> _bitSize; \
		index > offset; --index)
	{
		auto& target = _vector[index - 1];
		auto source = _vector[index - 1 - offset];
		target = (source & lowMask) << position;

		// 第一条件：避免未定义行为之位移计数为负或过大
		if (position > 0 && index - offset > 1)
		{
			source = _vector[index - 2 - offset];
			target |= (source & highMask) >> maxPosition - position;
		}
	}

	if (index > 0)
		std::memset(_vector.data(), 0, sizeof(ValueType) * index);
	return *this;
}

template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::operator>>=(SizeType _position) noexcept
{
	constexpr auto maxPosition = _maxPosition + 1;
	decltype(_position) position = _position & _maxPosition;

	using ValueType = std::remove_const_t<decltype(_maxElement)>;
	ValueType highMask = _maxElement << position;
	decltype(highMask) lowMask = ~highMask;

	auto size = _vector.size();
	decltype(size) index = 0;
	for (decltype(size) offset = _position >> _bitSize, \
		end = offset < size ? size - offset : 0; \
		index < end; ++index)
	{
		auto& target = _vector[index];
		auto source = _vector[index + offset];
		target = (source & highMask) >> position;

		// 第一条件：避免未定义行为之位移计数为负或过大
		if (auto cursor = index + offset + 1; \
			position > 0 && cursor < size)
		{
			source = _vector[cursor];
			target |= (source & lowMask) << maxPosition - position;
		}
	}

	if (index < size)
		std::memset(_vector.data() + index, 0, sizeof(ValueType) * (size - index));
	return *this;
}

// 统计有效位
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::count() const noexcept -> SizeType
{
	constexpr unsigned char maxChar = ~static_cast<unsigned char>('\0');
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
	for (auto element : _vector)
		for (auto index = sizeof element; index > 0; --index)
		{
			counter += bitTable[element & maxChar];
			element >>= CHAR_BIT;
		}
	return counter;
}

// 改变元素数量
template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::resize(SizeType _size, bool _forced)
{
	if (_forced || _size > _vector.size())
		_vector.resize(_size, 0);
	return *this;
}

// 设置指定位
template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::set(SizeType _position, bool _value)
{
	if (!_value)
		return reset(_position);

	reserve(capacity(_position));

	_vector[_position >> _bitSize] |= generate(_position);
	return *this;
}

// 设置指定范围
template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::set(SizeType _begin, SizeType _end, bool _value)
{
	if (!_value)
		return reset(_begin, _end);

	if (_begin >= _end)
		return *this;

	reserve(_end);

	traverse(_begin, _end, \
		[](ValueType& _element, ValueType _mask) noexcept { _element |= _mask; });
	return *this;
}

// 重置指定位
template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::reset(SizeType _position) noexcept
{
	if (size(capacity(_position)) <= _vector.size())
		_vector[_position >> _bitSize] &= ~generate(_position);
	return *this;
}

// 重置指定范围
template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::reset(SizeType _begin, SizeType _end) noexcept
{
	if (_begin >= _end \
		|| size(capacity(_begin)) > _vector.size())
		return *this;

	traverse(_begin, _end, \
		[](ValueType& _element, ValueType _mask) noexcept { _element &= ~_mask; });
	return *this;
}

// 翻转指定位
template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::flip(SizeType _position)
{
	reserve(capacity(_position));

	_vector[_position >> _bitSize] ^= generate(_position);
	return *this;
}

// 翻转指定范围
template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::flip(SizeType _begin, SizeType _end)
{
	if (_begin >= _end)
		return *this;

	reserve(_end);

	traverse(_begin, _end, \
		[](ValueType& _element, ValueType _mask) noexcept { _element ^= _mask; });
	return *this;
}

// 翻转所有位
template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::flip() noexcept
{
	for (auto& element : _vector)
		element = ~element;
	return *this;
}

// 复制指定范围
template <std::unsigned_integral _ValueType>
BitSet<_ValueType> BitSet<_ValueType>::copy(SizeType _begin, SizeType _end) const
{
	if (_begin >= _end)
		return BitSet();

	if (size(capacity(_begin)) > _vector.size())
		return BitSet();

	constexpr auto maxPosition = _maxPosition + 1;
	decltype(_begin) position = _begin & _maxPosition;

	using ValueType = std::remove_const_t<decltype(_maxElement)>;
	ValueType highMask = _maxElement << position;
	decltype(highMask) lowMask = ~highMask;

	_end = std::min(_end, maxPosition * _vector.size());
	auto capacity = _end - _begin;
	auto size = BitSet::size(capacity);
	BitSet bitSet(size);

	decltype(_begin) offset = _begin >> _bitSize;
	for (decltype(size) index = 0; index < size; ++index)
	{
		auto& target = bitSet._vector[index];
		auto source = _vector[index + offset];
		target = (source & highMask) >> position;

		// 第一条件：避免未定义行为之位移计数为负或过大
		if (auto cursor = index + offset + 1; \
			position > 0 && cursor < _vector.size())
		{
			source = _vector[cursor];
			target |= (source & lowMask) << maxPosition - position;
		}
	}

	auto mask = _maxElement;
	if (decltype(capacity) position = capacity & _maxPosition; position > 0)
		mask = ~(mask << position);

	auto& element = bitSet._vector.back();
	element &= mask;
	return bitSet;
}
