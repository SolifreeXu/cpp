/*
* 文件名称：LRUQueue.hpp
* 摘要：
* 1.页面置换算法：最近最久未使用(Least Recently Used)。
* 2.LRU队列按照最近访问顺序对元素排序，提供查找、放入、取出、清空等方法。
*
* 版本：v1.0.0
* 作者：许聪
* 邮箱：2592419242@qq.com
* 创建日期：2022年02月02日
*/

#pragma once

#include <utility>
#include <cstddef>
#include <optional>
#include <cstdint>
#include <map>

template <typename _KeyType, typename _ValueType>
class LRUQueue
{
public:
	using SizeType = std::size_t;
	using KeyType = _KeyType;
	using ValueType = _ValueType;

private:
	using QueueType = std::map<SizeType, KeyType>;
	using PairType = std::pair<ValueType, SizeType>;
	using PoolType = std::map<KeyType, PairType>;

	SizeType _capacity;
	SizeType _counter; // [0, SIZE_MAX)
	QueueType _queue; // count -> key
	PoolType _mappings; // key -> pair(value, count)

private:
	void insert(const KeyType& _key, const ValueType& _value);
	void insert(const KeyType& _key, ValueType&& _value);

	void erase() noexcept;
	void adjust();

public:
	// 若_capacity小于等于零，则无限制，否则为上限值
	LRUQueue(SizeType _capacity = 0) : _capacity(_capacity), _counter(0) {}

	auto capacity() const noexcept { return _capacity; }
	void reserve(SizeType _capacity) noexcept { this->_capacity = _capacity; }

	auto size() const noexcept { return _queue.size(); }
	bool empty() const noexcept { return _queue.empty(); }

	bool find(const KeyType& _key, ValueType& _value);
	std::optional<ValueType> find(const KeyType& _key)
	{
		ValueType value;
		return find(_key, value) ? value : std::optional<ValueType>();
	}

	void push(const KeyType& _key, const ValueType& _value);
	void push(const KeyType& _key, ValueType&& _value);

	std::optional<ValueType> pop(const KeyType& _key);
	void clear() noexcept;
};

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::insert(const KeyType& _key, const ValueType& _value)
{
	auto count = _counter++;
	_queue.emplace(count, _key);
	_mappings.emplace(_key, std::make_pair(_value, count));
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::insert(const KeyType& _key, ValueType&& _value)
{
	auto count = _counter++;
	_queue.emplace(count, _key);
	_mappings.emplace(_key, std::make_pair(std::forward<ValueType>(_value), count));
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::erase() noexcept
{
	while (_capacity > 0 && _queue.size() >= _capacity)
	{
		auto iterator = _queue.begin();
		_mappings.erase(iterator->second);
		_queue.erase(iterator);
	}
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::adjust()
{
	if (_counter < SIZE_MAX)
		return;

	_counter = 0;
	for (auto iterQueue = _queue.begin(); iterQueue != _queue.end(); ++iterQueue)
	{
		auto count = _counter++;
		const_cast<SizeType&>(iterQueue->first) = count;

		if (auto iterMappings = _mappings.find(iterQueue->second); \
			iterMappings != _mappings.end())
			iterMappings->second.second = count;
	}
}

template <typename _KeyType, typename _ValueType>
bool LRUQueue<_KeyType, _ValueType>::find(const KeyType& _key, ValueType& _value)
{
	auto iterator = _mappings.find(_key);
	if (iterator == _mappings.end())
		return false;

	_value = iterator->second.first;
	_queue.erase(iterator->second.second);

	auto count = _counter++;
	iterator->second.second = count;
	_queue.emplace(count, _key);

	adjust();
	return true;
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
auto LRUQueue<_KeyType, _ValueType>::pop(const KeyType& _key) -> std::optional<ValueType>
{
	auto iterator = _mappings.find(_key);
	if (iterator == _mappings.end())
		return std::nullopt;

	std::optional<ValueType> value = iterator->second.first;
	_queue.erase(iterator->second.second);
	_mappings.erase(iterator);
	return value;
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::clear() noexcept
{
	_mappings.clear();
	_queue.clear();
	_counter = 0;
}
