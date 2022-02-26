/*
* 文件名称：LRUQueue.hpp
* 语言标准：C++20
* 
* 创建日期：2022年02月02日
* 更新日期：2022年02月26日
* 
* 摘要
* 1.页面置换算法：最近最久未使用(Least Recently Used)。
* 2.LRU队列按照最近访问顺序对元素排序，提供查找、放入、取出、清空等方法。
* 
* 作者：许聪
* 邮箱：solifree@qq.com
* 
* 版本：v1.0.0
*/

#pragma once

#include <type_traits>
#include <utility>
#include <optional>
#include <cstdint>
#include <vector>
#include <map>
#include <unordered_map>

template <typename _KeyType, typename _ValueType>
class LRUQueue
{
public:
	using KeyType = _KeyType;
	using ValueType = _ValueType;
	using PairType = std::pair<KeyType, ValueType>;
	using VectorType = std::vector<PairType>;
	using SizeType = VectorType::size_type;

private:
	using QueueType = std::map<SizeType, KeyType>;
	using PoolType = std::unordered_map<KeyType, std::pair<ValueType, SizeType>>;

private:
	SizeType _capacity;
	SizeType _counter; // [0, SIZE_MAX)
	QueueType _queue; // count -> key
	PoolType _pool; // key -> pair(value, count)

private:
	void insert(const KeyType& _key, const ValueType& _value);
	void insert(const KeyType& _key, ValueType&& _value);

	void erase();

	void adjust();

public:
	// 若_capacity小于等于零，则无限制，否则其为上限值
	LRUQueue(decltype(_capacity) _capacity = 0)
		: _capacity(_capacity), _counter(0) {}

	auto capacity() const noexcept { return _capacity; }
	void reserve(decltype(_capacity) _capacity) noexcept
	{
		this->_capacity = _capacity;
	}

	auto size() const noexcept { return _queue.size(); }
	bool empty() const noexcept { return _queue.empty(); }

	bool contain(const KeyType& _key) const
	{
		return _pool.contains(_key);
	}

	bool find(const KeyType& _key, ValueType& _value);
	std::optional<ValueType> find(const KeyType& _key);

	void push(const KeyType& _key, const ValueType& _value);
	void push(const KeyType& _key, ValueType&& _value);

	bool pop(const KeyType& _key, ValueType& _value);
	std::optional<ValueType> pop(const KeyType& _key);

	bool pop(VectorType& _vector);

	void clear() noexcept;
};

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::insert(const KeyType& _key, const ValueType& _value)
{
	auto count = _counter++;
	_queue.emplace(count, _key);
	_pool.emplace(_key, std::make_pair(_value, count));
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::insert(const KeyType& _key, ValueType&& _value)
{
	auto count = _counter++;
	_queue.emplace(count, _key);
	_pool.emplace(_key, std::make_pair(std::forward<ValueType>(_value), count));
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::erase()
{
	while (_capacity > 0 && _queue.size() >= _capacity)
	{
		auto iterator = _queue.begin();
		_pool.erase(iterator->second);
		_queue.erase(iterator);
	}
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::adjust()
{
	if (_counter < SIZE_MAX)
		return;

	_counter = 0;
	for (auto& [count, key] : _queue)
	{
		using SizeType = std::remove_const_t<std::remove_reference_t<decltype(count)>>;
		const_cast<SizeType&>(count) = _counter++;

		if (auto iterator = _pool.find(key); \
			iterator != _pool.end())
			iterator->second.second = count;
	}
}

template <typename _KeyType, typename _ValueType>
bool LRUQueue<_KeyType, _ValueType>::find(const KeyType& _key, ValueType& _value)
{
	auto iterator = _pool.find(_key);
	if (iterator == _pool.end())
		return false;

	auto& [value, count] = iterator->second;
	_value = value;

	_queue.erase(count);
	count = _counter++;
	_queue.emplace(count, _key);

	adjust();
	return true;
}

template <typename _KeyType, typename _ValueType>
auto LRUQueue<_KeyType, _ValueType>::find(const KeyType& _key) -> std::optional<ValueType>
{
	auto iterator = _pool.find(_key);
	if (iterator == _pool.end())
		return std::nullopt;

	auto& [value, count] = iterator->second;
	_queue.erase(count);
	count = _counter++;
	_queue.emplace(count, _key);

	adjust();
	return value;
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::push(const KeyType& _key, const ValueType& _value)
{
	erase();
	insert(_key, _value);
	adjust();
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::push(const KeyType& _key, ValueType&& _value)
{
	erase();
	insert(_key, std::forward<ValueType>(_value));
	adjust();
}

template <typename _KeyType, typename _ValueType>
bool LRUQueue<_KeyType, _ValueType>::pop(const KeyType& _key, ValueType& _value)
{
	auto iterator = _pool.find(_key);
	if (iterator == _pool.end())
		return false;

	auto& [value, count] = iterator->second;
	_value = value;

	_queue.erase(count);
	_pool.erase(iterator);
	return true;
}

template <typename _KeyType, typename _ValueType>
auto LRUQueue<_KeyType, _ValueType>::pop(const KeyType& _key) -> std::optional<ValueType>
{
	auto iterator = _pool.find(_key);
	if (iterator == _pool.end())
		return std::nullopt;

	auto& [value, count] = iterator->second;
	std::optional result = value;

	_queue.erase(count);
	_pool.erase(iterator);
	return result;
}

template <typename _KeyType, typename _ValueType>
bool LRUQueue<_KeyType, _ValueType>::pop(VectorType& _vector)
{
	if (_queue.empty())
		return false;

	_vector.reserve(_vector.size() + _queue.size());
	for (const auto& [count, key] : _queue)
		if (auto iterator = _pool.find(key); \
			iterator != _pool.end())
			_vector.emplace_back(iterator->first, iterator->second.first);

	clear();
	return true;
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::clear() noexcept
{
	_pool.clear();
	_queue.clear();
	_counter = 0;
}
