#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <climits>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>

template <std::unsigned_integral _BitSet>
_BitSet generateBit(std::size_t _position) noexcept
{
	return static_cast<_BitSet>(1) << _position;
}

template <std::unsigned_integral _BitSet>
bool existBit(_BitSet _bitSet, std::size_t _position) noexcept
{
	return _position < sizeof _bitSet ? \
		(_bitSet & generateBit<_BitSet>(_position)) > 0 : false;
}

template <std::unsigned_integral _BitSet>
void resetBit(_BitSet& _bitSet, std::size_t _position) noexcept
{
	if (_position < sizeof _bitSet)
		_bitSet &= ~generateBit<_BitSet>(_position);
}

template <std::unsigned_integral _BitSet>
void resetBit(_BitSet& _bitSet) noexcept
{
	_bitSet = 0;
}

template <std::unsigned_integral _BitSet>
void setBit(_BitSet& _bitSet, std::size_t _position, \
	bool _value = true) noexcept
{
	if (not _value) resetBit(_bitSet, _position);
	else if (_position < sizeof _bitSet)
		_bitSet |= generateBit<_BitSet>(_position);
}

template <std::unsigned_integral _BitSet>
void setBit(_BitSet& _bitSet, bool _value = true) noexcept
{
	if (not _value) resetBit(_bitSet);
	else _bitSet = ~static_cast<_BitSet>(0);
}

template <std::unsigned_integral _ValueType>
class BitSet final
{
	using Vector = std::vector<_ValueType>;
	using Functor = \
		std::function<void(_ValueType&, _ValueType)>;

public:
	using ValueType = _ValueType;
	using SizeType = Vector::size_type;

private:
	// 单元素最大值
	static constexpr ValueType MAX_ELEMENT = \
		~static_cast<ValueType>(0);

	// 单元素之位置最大值
	static constexpr SizeType MAX_POSITION = \
		CHAR_BIT * sizeof(ValueType) - 1;

	// 单元素之位数量对数
	static const SizeType BIT_SIZE_LOG2;

private:
	Vector _vector; // 元素向量

public:
	friend auto operator&(const BitSet& _left, \
		const BitSet& _right)
	{
		return BitSet(_left) &= _right;
	}

	friend auto operator|(const BitSet& _left, \
		const BitSet& _right)
	{
		return BitSet(_left) |= _right;
	}

	friend auto operator^(const BitSet& _left, \
		const BitSet& _right)
	{
		return BitSet(_left) ^= _right;
	}

private:
	// 位容量
	static constexpr auto capacity(SizeType _position) noexcept
	{
		return _position + 1;
	}

	// 元素数量
	static constexpr auto size(SizeType _position) noexcept
	{
		return (_position >> BIT_SIZE_LOG2) + 1;
	}

	// 统计有效位
	static constexpr SizeType count(ValueType _element) noexcept;

	// 生成单元素
	static constexpr ValueType generate(SizeType _position) noexcept
	{
		_position &= MAX_POSITION;
		return static_cast<ValueType>(1) << _position;
	}

private:
	// 预留空间
	void reserve(SizeType _position);

	// 判断所有元素值
	bool all(ValueType _element) const noexcept;

	// 遍历指定范围元素
	void traverse(SizeType _begin, SizeType _end, \
		Functor _functor) noexcept;

public:
	BitSet(SizeType _size = 0) : _vector(_size, 0)
	{
		static_assert(count(static_cast<ValueType>(sizeof(ValueType))) == 1, \
			"The size of ValueType must be a power of 2.");
	}

	BitSet(const ValueType* _data, SizeType _size) : \
		_vector(_data, _data + _size) {}

	bool operator==(const BitSet& _bitSet) const noexcept;

	// C++ 20 != 采用 == 推导
	//bool operator!=(const BitSet& _bitSet) const noexcept
	//{
	//	return not (*this == _bitSet);
	//}

	bool operator[](SizeType _position) const noexcept
	{
		return exist(_position);
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

	// 存在有效位
	bool exist(SizeType _position) const noexcept;

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
		return all(MAX_ELEMENT);
	}

	// 任意位有效
	bool any() const noexcept
	{
		return not all(0);
	}

	// 无有效位
	bool none() const noexcept
	{
		return all(0);
	}

	// 设置指定位
	BitSet& set(SizeType _position, bool _value = true);

	// 设置指定范围
	BitSet& set(SizeType _begin, SizeType _end, \
		bool _value = true);

	// 设置所有位
	BitSet& set() noexcept
	{
		_vector.assign(_vector.size(), MAX_ELEMENT);
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
const typename BitSet<_ValueType>::SizeType BitSet<_ValueType>::BIT_SIZE_LOG2 = \
	static_cast<SizeType>(std::log2(CHAR_BIT * sizeof(ValueType)));

// 统计有效位
template <std::unsigned_integral _ValueType>
constexpr auto BitSet<_ValueType>::count(ValueType _element) noexcept -> SizeType
{
	constexpr unsigned char MAX_CHARACTER = ~static_cast<unsigned char>('\0');
	const char* const BIT_TABLE = \
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
	for (auto index = sizeof _element; index > 0; --index)
	{
		counter += BIT_TABLE[_element & MAX_CHARACTER];
		_element >>= CHAR_BIT;
	}
	return counter;
}

// 预留空间
template <std::unsigned_integral _ValueType>
void BitSet<_ValueType>::reserve(SizeType _position)
{
	if (auto size = BitSet::size(_position); \
		size > _vector.size())
		_vector.resize(size, 0);
}

// 判断所有元素值
template <std::unsigned_integral _ValueType>
bool BitSet<_ValueType>::all(ValueType _element) const noexcept
{
	for (auto element : _vector)
		if (element != _element) return false;
	return true;
}

// 遍历指定范围元素
template <std::unsigned_integral _ValueType>
void BitSet<_ValueType>::traverse(SizeType _begin, SizeType _end, \
	Functor _functor) noexcept
{
	if (not _functor) return;

	auto beginPosition = _begin & MAX_POSITION;
	auto endPosition = _end & MAX_POSITION;

	ValueType beginMask = MAX_ELEMENT << beginPosition;
	ValueType endMask = ~(MAX_ELEMENT << endPosition);

	auto size = std::min(_vector.size(), BitSet::size(_end - 1));
	_begin >>= BIT_SIZE_LOG2;
	_end >>= BIT_SIZE_LOG2;

	for (auto index = _begin; index < size; ++index)
	{
		auto mask = MAX_ELEMENT;
		if (index == _begin) mask &= beginMask;

		if (index == _end) mask &= endMask;

		_functor(_vector[index], mask);
	}
}

template <std::unsigned_integral _ValueType>
bool BitSet<_ValueType>::operator==(const BitSet& _bitSet) const noexcept
{
	if (this == &_bitSet) return true;

	auto bitSet = this;
	auto size = _bitSet._vector.size();
	if (this->_vector.size() < size)
	{
		bitSet = &_bitSet;
		size = this->_vector.size();
	}

	auto left = this->_vector.data();
	auto right = _bitSet._vector.data();
	if (std::memcmp(left, right, sizeof *left * size) != 0) return false;

	for (auto index = size; index < bitSet->_vector.size(); ++index)
		if (bitSet->_vector[index] != 0) return false;
	return true;
}

template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::operator&=(const BitSet& _bitSet) -> BitSet&
{
	if (this == &_bitSet) return *this;

	auto size = std::max(this->_vector.size(), _bitSet._vector.size());
	resize(size);

	decltype(size) index = 0;
	for (; index < _bitSet._vector.size(); ++index)
		this->_vector[index] &= _bitSet._vector[index];

	if (index < size)
	{
		auto data = this->_vector.data();
		std::memset(data + index, 0, sizeof *data * (size - index));
	}
	return *this;
}

template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::operator|=(const BitSet& _bitSet) -> BitSet&
{
	if (this == &_bitSet) return *this;

	auto size = std::max(this->_vector.size(), _bitSet._vector.size());
	resize(size);

	for (decltype(size) index = 0; \
		index < _bitSet._vector.size(); ++index)
		this->_vector[index] |= _bitSet._vector[index];
	return *this;
}

template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::operator^=(const BitSet& _bitSet) -> BitSet&
{
	if (this == &_bitSet)
	{
		auto data = this->_vector.data();
		auto size = this->_vector.size();
		std::memset(data, 0, sizeof *data * size);
		return *this;
	}

	auto size = std::max(this->_vector.size(), _bitSet._vector.size());
	resize(size);

	for (decltype(size) index = 0; \
		index < _bitSet._vector.size(); ++index)
		this->_vector[index] ^= _bitSet._vector[index];
	return *this;
}

template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::operator<<=(SizeType _position) noexcept \
-> BitSet&
{
	if (_position <= 0) return *this;

	constexpr auto CAPACITY = capacity(MAX_POSITION);

	auto offset = _position >> BIT_SIZE_LOG2;
	_position &= MAX_POSITION;

	ValueType lowMask = MAX_ELEMENT >> _position;
	ValueType highMask = ~lowMask;

	auto index = _vector.size();
	for (; index > offset; --index)
	{
		auto cursor = index - 1;
		auto& target = _vector[cursor];
		cursor -= offset;
		auto source = _vector[cursor];
		target = (source & lowMask) << _position;

		// 第一条件：避免未定义行为之位移计数为负或过大
		if (_position > 0 and cursor > 0)
		{
			auto source = _vector[cursor - 1];
			target |= (source & highMask) >> CAPACITY - _position;
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
auto BitSet<_ValueType>::operator>>=(SizeType _position) noexcept \
-> BitSet&
{
	if (_position <= 0) return *this;

	constexpr auto CAPACITY = capacity(MAX_POSITION);

	auto offset = _position >> BIT_SIZE_LOG2;
	_position &= MAX_POSITION;

	ValueType highMask = MAX_ELEMENT << _position;
	ValueType lowMask = ~highMask;

	auto size = _vector.size();
	decltype(size) limit = offset < size ? size - offset : 0;
	decltype(limit) index = 0;
	for (; index < limit; ++index)
	{
		auto& target = _vector[index];
		auto cursor = index + offset;
		auto source = _vector[cursor];
		target = (source & highMask) >> _position;

		// 第一条件：避免未定义行为之位移计数为负或过大
		if (_position > 0 and ++cursor < size)
		{
			auto source = _vector[cursor];
			target |= (source & lowMask) << CAPACITY - _position;
		}
	}

	if (index < size)
	{
		auto data = _vector.data();
		std::memset(data + index, 0, sizeof *data * (size - index));
	}
	return *this;
}

// 存在有效位
template <std::unsigned_integral _ValueType>
bool BitSet<_ValueType>::exist(SizeType _position) const noexcept
{
	if (size(_position) > _vector.size()) return false;

	auto element = _vector[_position >> BIT_SIZE_LOG2];
	return (element & generate(_position)) > 0;
}

// 改变元素数量
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::resize(SizeType _size, \
	bool _forced) -> BitSet&
{
	if (_forced or _size > _vector.size())
		_vector.resize(_size, 0);
	return *this;
}

// 统计有效位
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::count() const noexcept \
-> SizeType
{
	SizeType counter = 0;
	for (auto element : _vector)
		counter += count(element);
	return counter;
}

// 设置指定位
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::set(SizeType _position, bool _value) \
-> BitSet&
{
	if (not _value) return reset(_position);

	reserve(_position);

	_vector[_position >> BIT_SIZE_LOG2] |= generate(_position);
	return *this;
}

// 设置指定范围
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::set(SizeType _begin, SizeType _end, \
	bool _value) -> BitSet&
{
	if (not _value) return reset(_begin, _end);

	if (_begin < _end)
	{
		reserve(_end - 1);

		traverse(_begin, _end, \
			[](ValueType& _element, ValueType _mask) noexcept
			{ _element |= _mask; });
	}
	return *this;
}

// 重置指定位
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::reset(SizeType _position) noexcept -> BitSet&
{
	if (size(_position) <= _vector.size())
		_vector[_position >> BIT_SIZE_LOG2] &= ~generate(_position);
	return *this;
}

// 重置指定范围
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::reset(SizeType _begin, SizeType _end) noexcept \
-> BitSet&
{
	if (_begin < _end and size(_begin) <= _vector.size())
		traverse(_begin, _end, \
			[](ValueType& _element, ValueType _mask) noexcept
			{ _element &= ~_mask; });
	return *this;
}

// 翻转指定位
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::flip(SizeType _position) -> BitSet&
{
	reserve(_position);

	_vector[_position >> BIT_SIZE_LOG2] ^= generate(_position);
	return *this;
}

// 翻转指定范围
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::flip(SizeType _begin, SizeType _end) \
-> BitSet&
{
	if (_begin < _end)
	{
		reserve(_end - 1);

		traverse(_begin, _end, \
			[](ValueType& _element, ValueType _mask) noexcept
			{ _element ^= _mask; });
	}
	return *this;
}

// 翻转所有位
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::flip() noexcept \
-> BitSet&
{
	for (auto& element : _vector)
		element = ~element;
	return *this;
}

// 复制指定范围
template <std::unsigned_integral _ValueType>
auto BitSet<_ValueType>::copy(SizeType _begin, SizeType _end) const \
-> BitSet
{
	if (_begin >= _end) return BitSet();

	if (size(_begin) > _vector.size()) return BitSet();

	constexpr auto CAPACITY = capacity(MAX_POSITION);

	auto position = _begin & MAX_POSITION;

	ValueType highMask = MAX_ELEMENT << position;
	ValueType lowMask = ~highMask;

	auto size = _vector.size();
	_end = BitSet::size(_end - 1) <= size ? _end : CAPACITY * size;
	auto difference = _end - _begin - 1;

	size = BitSet::size(difference);
	BitSet bitSet(size);

	auto offset = _begin >> BIT_SIZE_LOG2;
	for (decltype(size) index = 0; index < size; ++index)
	{
		auto& target = bitSet._vector[index];
		auto cursor = index + offset;
		auto source = this->_vector[cursor];
		target = (source & highMask) >> position;

		// 第一条件：避免未定义行为之位移计数为负或过大
		if (position > 0 and ++cursor < this->_vector.size())
		{
			auto source = this->_vector[cursor];
			target |= (source & lowMask) << CAPACITY - position;
		}
	}

	// 两种算法等价
	//position = MAX_POSITION - (difference & MAX_POSITION);
	position = MAX_POSITION ^ (difference & MAX_POSITION);
	auto mask = MAX_ELEMENT >> position;

	auto& element = bitSet._vector.back();
	element &= mask;
	return bitSet;
}
