#pragma once

#include <utility>
#include <optional>
#include <vector>
#include <map>

template <typename _TimeType, typename _IndexType, typename _ElementType>
class TimeoutQueue final
{
public:
	using TimeType = _TimeType;
	using IndexType = _IndexType;
	using ElementType = _ElementType;

	using PairType = std::pair<ElementType, TimeType>;
	using VectorType = std::vector<std::pair<IndexType, ElementType>>;
	using SizeType = VectorType::size_type;

private:
	using QueueType = std::multimap<TimeType, IndexType>;
	using TableType = std::map<IndexType, PairType>;

private:
	SizeType _capacity;
	QueueType _queue;
	TableType _table;

private:
	void erase(TimeType _time, IndexType _index);

public:
	TimeoutQueue(decltype(_capacity) _capacity = 0) : \
		_capacity(_capacity) {}

	auto capacity() const noexcept { return _capacity; }
	void reserve(decltype(_capacity) _capacity) noexcept
	{
		this->_capacity = _capacity;
	}

	bool empty() const noexcept { return _queue.empty(); }
	auto size() const noexcept { return _queue.size(); }

	bool exist(IndexType _index) const
	{
		return _table.contains(_index);
	}

	const PairType* find(IndexType _index) const;

	bool push(TimeType _time, IndexType _index, \
		const ElementType& _element);
	bool push(TimeType _time, IndexType _index, \
		ElementType&& _element);

	bool pop(IndexType _index, ElementType& _element);
	std::optional<ElementType> pop(IndexType _index);

	bool pop(TimeType _time, VectorType& _vector);
	bool pop(VectorType& _vector);

	void clear() noexcept
	{
		_queue.clear();
		_table.clear();
	}
};

template <typename _TimeType, typename _IndexType, typename _ElementType>
void TimeoutQueue<_TimeType, _IndexType, _ElementType>::erase(TimeType _time, \
	IndexType _index)
{
	auto iterator = _queue.lower_bound(_time);
	if (iterator == _queue.end()) return;

	for (auto end = _queue.upper_bound(_time); iterator != end; ++iterator)
		if (iterator->second == _index)
		{
			_queue.erase(iterator);
			break;
		}
}

template <typename _TimeType, typename _IndexType, typename _ElementType>
auto TimeoutQueue<_TimeType, _IndexType, _ElementType>::find(IndexType _index) const \
-> const PairType*
{
	auto iterator = _table.find(_index);
	return iterator != _table.end() ? &iterator->second : nullptr;
}

template <typename _TimeType, typename _IndexType, typename _ElementType>
bool TimeoutQueue<_TimeType, _IndexType, _ElementType>::push(TimeType _time, \
	IndexType _index, const ElementType& _element)
{
	if (_capacity > 0 and size() >= _capacity) return false;
	if (exist(_index)) return false;

	_queue.emplace(_time, _index);
	_table.emplace(_index, std::make_pair(_element, _time));
	return true;
}

template <typename _TimeType, typename _IndexType, typename _ElementType>
bool TimeoutQueue<_TimeType, _IndexType, _ElementType>::push(TimeType _time, \
	IndexType _index, ElementType&& _element)
{
	if (_capacity > 0 and size() >= _capacity) return false;
	if (exist(_index)) return false;

	_queue.emplace(_time, _index);
	_table.emplace(_index, \
		std::make_pair(std::forward<ElementType>(_element), _time));
	return true;
}

template <typename _TimeType, typename _IndexType, typename _ElementType>
bool TimeoutQueue<_TimeType, _IndexType, _ElementType>::pop(IndexType _index, \
	ElementType& _element)
{
	auto iterator = _table.find(_index);
	if (iterator == _table.end()) return false;

	auto& [element, time] = iterator->second;
	_element = std::move(element);

	erase(time, _index);
	_table.erase(iterator);
	return true;
}

template <typename _TimeType, typename _IndexType, typename _ElementType>
auto TimeoutQueue<_TimeType, _IndexType, _ElementType>::pop(IndexType _index) \
-> std::optional<ElementType>
{
	auto iterator = _table.find(_index);
	if (iterator == _table.end()) return std::nullopt;

	auto& [element, time] = iterator->second;
	std::optional result = std::move(element);

	erase(time, _index);
	_table.erase(iterator);
	return result;
}

template <typename _TimeType, typename _IndexType, typename _ElementType>
bool TimeoutQueue<_TimeType, _IndexType, _ElementType>::pop(TimeType _time, \
	VectorType& _vector)
{
	auto size = _vector.size();
	for (auto iterator = _queue.begin(), end = _queue.upper_bound(_time); \
		iterator != end; iterator = _queue.erase(iterator))
	{
		const auto& index = iterator->second;
		if (auto iterator = _table.find(index); iterator != _table.end())
		{
			_vector.emplace_back(iterator->first, \
				std::move(iterator->second.first));
			_table.erase(iterator);
		}
	}
	return _vector.size() > size;
}

template <typename _TimeType, typename _IndexType, typename _ElementType>
bool TimeoutQueue<_TimeType, _IndexType, _ElementType>::pop(VectorType& _vector)
{
	if (empty()) return false;

	_vector.reserve(_vector.size() + size());
	for (const auto& [_, index] : _queue)
		if (auto iterator = _table.find(index); iterator != _table.end())
			_vector.emplace_back(iterator->first, \
				std::move(iterator->second.first));

	clear();
	return true;
}
