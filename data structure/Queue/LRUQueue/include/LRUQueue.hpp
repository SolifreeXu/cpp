#pragma once

#include <type_traits>
#include <utility>
#include <optional>
#include <cstdint>
#include <vector>
#include <map>
#include <unordered_map>

template <typename _KeyType, typename _ValueType>
class LRUQueue final
{
public:
	using KeyType = _KeyType;
	using ValueType = _ValueType;

	using PairType = std::pair<KeyType, ValueType>;
	using VectorType = std::vector<PairType>;
	using SizeType = VectorType::size_type;

private:
	using QueueType = std::map<SizeType, KeyType>;
	using TableType = std::unordered_map<KeyType, \
		std::pair<ValueType, SizeType>>;

private:
	SizeType _capacity;
	SizeType _counter; // [0, SIZE_MAX)
	QueueType _queue; // count -> key
	TableType _table; // key -> pair(value, count)

private:
	void insert(const KeyType& _key, const ValueType& _value);
	void insert(const KeyType& _key, ValueType&& _value);

	void erase();

	void adjust();

public:
	// 若_capacity小于等于零，则无限制，否则其为上限值
	LRUQueue(decltype(_capacity) _capacity = 0) : \
		_capacity(_capacity), _counter(0) {}

	auto capacity() const noexcept { return _capacity; }
	void reserve(decltype(_capacity) _capacity) noexcept
	{
		this->_capacity = _capacity;
	}

	bool empty() const noexcept { return _queue.empty(); }
	auto size() const noexcept { return _queue.size(); }

	bool exist(const KeyType& _key) const
	{
		return _table.contains(_key);
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
void LRUQueue<_KeyType, _ValueType>::insert(const KeyType& _key, \
	const ValueType& _value)
{
	auto count = _counter++;
	_queue.emplace(count, _key);
	_table.emplace(_key, std::make_pair(_value, count));
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::insert(const KeyType& _key, \
	ValueType&& _value)
{
	auto count = _counter++;
	_queue.emplace(count, _key);
	_table.emplace(_key, \
		std::make_pair(std::forward<ValueType>(_value), count));
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::erase()
{
	if (_capacity <= 0) return;

	while (size() >= _capacity)
	{
		auto iterator = _queue.cbegin();
		_table.erase(iterator->second);
		_queue.erase(iterator);
	}
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::adjust()
{
	if (_counter < SIZE_MAX) return;

	_counter = 0;
	for (auto& [count, key] : _queue)
	{
		using SizeType = std::remove_const_t<std::remove_reference_t<decltype(count)>>;
		const_cast<SizeType&>(count) = _counter++;

		if (auto iterator = _table.find(key); \
			iterator != _table.end())
			iterator->second.second = count;
	}
}

template <typename _KeyType, typename _ValueType>
bool LRUQueue<_KeyType, _ValueType>::find(const KeyType& _key, \
	ValueType& _value)
{
	auto iterator = _table.find(_key);
	if (iterator == _table.end()) return false;

	auto& [value, count] = iterator->second;
	_value = value;

	auto node = _queue.extract(count);
	node.key() = count = _counter++;
	_queue.insert(std::move(node));

	adjust();
	return true;
}

template <typename _KeyType, typename _ValueType>
auto LRUQueue<_KeyType, _ValueType>::find(const KeyType& _key) \
-> std::optional<ValueType>
{
	auto iterator = _table.find(_key);
	if (iterator == _table.end()) return std::nullopt;

	auto& [value, count] = iterator->second;
	auto node = _queue.extract(count);
	node.key() = count = _counter++;
	_queue.insert(std::move(node));

	adjust();
	return value;
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::push(const KeyType& _key, \
	const ValueType& _value)
{
	erase();
	insert(_key, _value);
	adjust();
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::push(const KeyType& _key, \
	ValueType&& _value)
{
	erase();
	insert(_key, std::forward<ValueType>(_value));
	adjust();
}

template <typename _KeyType, typename _ValueType>
bool LRUQueue<_KeyType, _ValueType>::pop(const KeyType& _key, \
	ValueType& _value)
{
	auto iterator = _table.find(_key);
	if (iterator == _table.end()) return false;

	auto& [value, count] = iterator->second;
	_value = std::move(value);

	_queue.erase(count);
	_table.erase(iterator);
	return true;
}

template <typename _KeyType, typename _ValueType>
auto LRUQueue<_KeyType, _ValueType>::pop(const KeyType& _key) \
-> std::optional<ValueType>
{
	auto iterator = _table.find(_key);
	if (iterator == _table.end()) return std::nullopt;

	auto& [value, count] = iterator->second;
	std::optional result = std::move(value);

	_queue.erase(count);
	_table.erase(iterator);
	return result;
}

template <typename _KeyType, typename _ValueType>
bool LRUQueue<_KeyType, _ValueType>::pop(VectorType& _vector)
{
	if (empty()) return false;

	_vector.reserve(_vector.size() + size());
	for (const auto& [count, key] : _queue)
		if (auto iterator = _table.find(key); \
			iterator != _table.end())
			_vector.emplace_back(iterator->first, \
				std::move(iterator->second.first));

	clear();
	return true;
}

template <typename _KeyType, typename _ValueType>
void LRUQueue<_KeyType, _ValueType>::clear() noexcept
{
	_table.clear();
	_queue.clear();
	_counter = 0;
}
