#pragma once

#include "Version.hpp"

#include <utility>
#include <list>
#include <unordered_map>

template <typename _KeyType, typename _ValueType>
class LRUQueue final
{
public:
	using KeyType = _KeyType;
	using ValueType = _ValueType;

	using PairType = std::pair<KeyType, ValueType>;
	using QueueType = std::list<PairType>;
#if CXX_VERSION >= CXX_2020
	using SizeType = QueueType::size_type;
#else
	using SizeType = typename QueueType::size_type;
#endif

private:
#if CXX_VERSION >= CXX_2020
	using Iterator = QueueType::iterator;
#else
	using Iterator = typename QueueType::iterator;
#endif
	using TableType = std::unordered_map<KeyType, Iterator>;

private:
	SizeType _capacity;
	QueueType _queue; // pair(key, value)
	TableType _table; // key -> iterator(_queue)

private:
	void move(Iterator& _iterator);

	void erase();

public:
	// 若_capacity小于等于零，则无限制，否则其为上限值
	LRUQueue(decltype(_capacity) _capacity = 0) : \
		_capacity(_capacity) {}

	auto capacity() const noexcept { return _capacity; }
	void reserve(decltype(_capacity) _capacity) noexcept
	{
		this->_capacity = _capacity;
	}

	bool empty() const noexcept { return _queue.empty(); }
	auto size() const noexcept { return _queue.size(); }

	bool exist(const KeyType& _key) const
	{
#if CXX_VERSION >= CXX_2020
		return _table.contains(_key);
#else
		return _table.find(_key) != _table.end();
#endif
	}

	const ValueType* find(const KeyType& _key);

	void push(const KeyType& _key, const ValueType& _value);
	void push(const KeyType& _key, ValueType&& _value);

	bool pop(const KeyType& _key, ValueType& _value);
	void pop(const KeyType& _key);

	bool pop(QueueType& _queue);

	void clear() noexcept;
};

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::move(Iterator& _iterator)
{
	decltype(_queue) queue;
	queue.splice(queue.end(), _queue, _iterator);
	_queue.splice(_queue.end(), queue);
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::erase()
{
	if (_capacity <= 0) return;

	while (size() >= _capacity)
	{
		auto iterator = _queue.cbegin();
		_table.erase(iterator->first);
		_queue.erase(iterator);
	}
}

template <typename _KeyType, typename _ValueType>
auto LRUQueue<_KeyType, _ValueType>::find(const KeyType& _key) \
-> const ValueType*
{
	auto iterTable = _table.find(_key);
	if (iterTable == _table.end()) return nullptr;

	auto& iterQueue = iterTable->second;
	auto& value = iterQueue->second;

	move(iterQueue);
	return &value;
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::push(const KeyType& _key, \
	const ValueType& _value)
{
	auto iterTable = _table.find(_key);
	if (iterTable == _table.end())
	{
		erase();

		auto iterQueue = _queue.emplace(_queue.end(), \
			_key, _value);
		_table.emplace(_key, iterQueue);
	}
	else
	{
		auto& iterQueue = iterTable->second;
		iterQueue->second = _value;

		move(iterQueue);
	}
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::push(const KeyType& _key, \
	ValueType&& _value)
{
	auto iterTable = _table.find(_key);
	if (iterTable == _table.end())
	{
		erase();

		auto iterQueue = _queue.emplace(_queue.end(), \
			_key, std::forward<ValueType>(_value));
		_table.emplace(_key, iterQueue);
	}
	else
	{
		auto& iterQueue = iterTable->second;
		iterQueue->second = std::forward<ValueType>(_value);

		move(iterQueue);
	}
}

template <typename _KeyType, typename _ValueType>
bool LRUQueue<_KeyType, _ValueType>::pop(const KeyType& _key, \
	ValueType& _value)
{
	auto iterTable = _table.find(_key);
	if (iterTable == _table.end()) return false;

	auto& iterQueue = iterTable->second;
	_value = std::move(iterQueue->second);

	_queue.erase(iterQueue);
	_table.erase(iterTable);
	return true;
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::pop(const KeyType& _key)
{
	auto iterTable = _table.find(_key);
	if (iterTable != _table.end())
	{
		auto& iterQueue = iterTable->second;
		_queue.erase(iterQueue);
		_table.erase(iterTable);
	}
}

template <typename _KeyType, typename _ValueType>
bool LRUQueue<_KeyType, _ValueType>::pop(QueueType& _queue)
{
	if (empty()) return false;

	_queue.splice(_queue.end(), this->_queue);

	clear();
	return true;
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::clear() noexcept
{
	_table.clear();
	_queue.clear();
}
