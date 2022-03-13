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
	using FunctorType = std::function<void(_ValueType&, const _ValueType&)>;

public:
	using ValueType = _ValueType;
	using SizeType = VectorType::size_type;

private:
	static constexpr ValueType _maxElement = ~static_cast<ValueType>(0); // 单元素最大值
	static constexpr SizeType _maxPosition = CHAR_BIT * sizeof(ValueType) - 1; // 单元素之位置最大值
	static const SizeType _bitSize; // 单元素之位数量

private:
	VectorType _vector; // 元素向量

public:
	friend bool operator==(const BitSet& _left, const BitSet& _right) noexcept
	{
		return _left._vector == _right._vector;
	}

	// C++ 20 != 采用 == 推导
	//friend bool operator!=(const BitSet& _left, const BitSet& _right) noexcept
	//{
	//	return _left._vector != _right._vector;
	//}

	friend auto operator&(const BitSet& _left, const BitSet& _right)
	{
		return BitSet(_left) &= _right;
	}

	friend auto operator|(const BitSet& _left, const BitSet& _right)
	{
		return BitSet(_left) |= _right;
	}

	friend auto operator^(const BitSet& _left, const BitSet& _right)
	{
		return BitSet(_left) ^= _right;
	}

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
		ValueType element = 1;
		return element <<= _position & _maxPosition;
	}

private:
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

	auto operator<<(SizeType _position) const
	{
		return BitSet(*this) <<= _position;
	}

	auto operator>>(SizeType _position) const
	{
		return BitSet(*this) >>= _position;
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

	// 改变元素数量
	BitSet& resize(SizeType _size, bool _forced = false);

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
	auto beginPosition = _begin & _maxPosition;
	auto endPosition = _end & _maxPosition;

	using ValueType = std::remove_const_t<decltype(_maxElement)>;
	ValueType beginMask = _maxElement << beginPosition;
	ValueType endMask = ~(_maxElement << endPosition);

	auto size = std::min(_vector.size(), BitSet::size(_end));
	_begin >>= _bitSize;
	_end >>= _bitSize;

	for (auto index = _begin; index < size; ++index)
	{
		auto mask = _maxElement;
		if (index == _begin)
			mask &= beginMask;

		if (index == _end)
			mask &= endMask;

		_functor(_vector[index], mask);
	}
}

template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::operator&=(const BitSet& _bitSet)
{
	auto size = std::max(_vector.size(), _bitSet._vector.size());
	resize(size);

	decltype(size) index = 0;
	for (; index < _bitSet._vector.size(); ++index)
		_vector[index] &= _bitSet._vector[index];

	if (index < size)
	{
		auto data = _vector.data();
		std::memset(data + index, 0, sizeof *data * (size - index));
	}
	return *this;
}

template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::operator|=(const BitSet& _bitSet)
{
	auto size = std::max(_vector.size(), _bitSet._vector.size());
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
	auto offset = _position >> _bitSize;
	_position &= _maxPosition;

	using ValueType = std::remove_const_t<decltype(_maxElement)>;
	ValueType lowMask = _maxElement >> _position;
	ValueType highMask = ~lowMask;

	auto index = _vector.size();
	for (; index > offset; --index)
	{
		auto& target = _vector[index - 1];
		auto cursor = index - 1 - offset;
		const auto& source = _vector[cursor];
		target = (source & lowMask) << _position;

		// 第一条件：避免未定义行为之位移计数为负或过大
		if (_position > 0 && cursor > 0)
		{
			const auto& source = _vector[cursor - 1];
			target |= (source & highMask) >> maxPosition - _position;
		}
	}

	if (index > 0)
	{
		auto data = _vector.data();
		std::memset(data, 0, sizeof *data * index);
	}
	return *this;
}

template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::operator>>=(SizeType _position) noexcept
{
	constexpr auto maxPosition = _maxPosition + 1;
	auto offset = _position >> _bitSize;
	_position &= _maxPosition;

	using ValueType = std::remove_const_t<decltype(_maxElement)>;
	ValueType highMask = _maxElement << _position;
	ValueType lowMask = ~highMask;

	auto size = _vector.size();
	decltype(size) index = 0;
	for (decltype(index) limit = offset < size ? size - offset : 0; \
		index < limit; ++index)
	{
		auto& target = _vector[index];
		auto cursor = index + offset;
		const auto& source = _vector[cursor];
		target = (source & highMask) >> _position;

		// 第一条件：避免未定义行为之位移计数为负或过大
		if (_position > 0 && ++cursor < size)
		{
			const auto& source = _vector[cursor];
			target |= (source & lowMask) << maxPosition - _position;
		}
	}

	if (index < size)
	{
		auto data = _vector.data();
		std::memset(data + index, 0, sizeof *data * (size - index));
	}
	return *this;
}

// 改变元素数量
template <std::unsigned_integral _ValueType>
BitSet<_ValueType>& BitSet<_ValueType>::resize(SizeType _size, bool _forced)
{
	if (_forced || _size > _vector.size())
		_vector.resize(_size, 0);
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
		[](ValueType& _element, const ValueType& _mask) noexcept { _element |= _mask; });
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
		[](ValueType& _element, const ValueType& _mask) noexcept { _element &= ~_mask; });
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
		[](ValueType& _element, const ValueType& _mask) noexcept { _element ^= _mask; });
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
	auto position = _begin & _maxPosition;

	using ValueType = std::remove_const_t<decltype(_maxElement)>;
	ValueType highMask = _maxElement << position;
	ValueType lowMask = ~highMask;

	_end = std::min(_end, maxPosition * _vector.size());
	auto capacity = _end - _begin;
	auto size = BitSet::size(capacity);
	BitSet bitSet(size);

	auto offset = _begin >> _bitSize;
	for (decltype(size) index = 0; index < size; ++index)
	{
		auto& target = bitSet._vector[index];
		auto cursor = index + offset;
		const auto& source = _vector[cursor];
		target = (source & highMask) >> position;

		// 第一条件：避免未定义行为之位移计数为负或过大
		if (position > 0 && ++cursor < _vector.size())
		{
			const auto& source = _vector[cursor];
			target |= (source & lowMask) << maxPosition - position;
		}
	}

	auto mask = _maxElement;
	if ((position = capacity & _maxPosition) > 0)
		mask = ~(mask << position);

	auto& element = bitSet._vector.back();
	element &= mask;
	return bitSet;
}
