#pragma once

#include <optional>
#include <utility>
#include <map>
#include <vector>

template <typename _TimeType, typename _Element>
class TimeoutQueue final
{
public:
	using TimeType = _TimeType;
	using Element = _Element;

	using Vector = std::vector<Element>;
	using SizeType = Vector::size_type;

private:
	using QueueType = std::multimap<TimeType, Element>;
	using TableType = std::map<Element, TimeType>;

private:
	SizeType _capacity;
	QueueType _queue;
	TableType _table;

private:
	void erase(TimeType _time, const Element& _element);

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

	bool exist(const Element& _element) const
	{
		return _table.contains(_element);
	}

	bool push(TimeType _time, const Element& _element);

	bool pop(const Element& _element);

	bool pop(TimeType _time, Vector& _vector);
	bool pop(Vector& _vector);

	void clear() noexcept
	{
		_queue.clear();
		_table.clear();
	}
};

template <typename _TimeType, typename _Element>
void TimeoutQueue<_TimeType, _Element>::erase(TimeType _time, \
	const Element& _element)
{
	auto iterator = _queue.lower_bound(_time);
	if (iterator == _queue.end()) return;

	for (auto end = _queue.upper_bound(_time); \
		iterator != end; ++iterator)
		if (iterator->second == _element)
		{
			_queue.erase(iterator);
			break;
		}
}

template <typename _TimeType, typename _Element>
bool TimeoutQueue<_TimeType, _Element>::push(TimeType _time, \
	const Element& _element)
{
	if (_capacity > 0 and size() >= _capacity) return false;
	if (exist(_element)) return false;

	_queue.emplace(_time, _element);
	_table.emplace(_element, _time);
	return true;
}

template <typename _TimeType, typename _Element>
bool TimeoutQueue<_TimeType, _Element>::pop(const Element& _element)
{
	auto iterator = _table.find(_element);
	if (iterator == _table.end()) return false;

	erase(iterator->second, _element);
	_table.erase(iterator);
	return true;
}

template <typename _TimeType, typename _Element>
bool TimeoutQueue<_TimeType, _Element>::pop(TimeType _time, Vector& _vector)
{
	auto size = _vector.size();
	for (auto iterator = _queue.begin(), end = _queue.upper_bound(_time); \
		iterator != end; iterator = _queue.erase(iterator))
	{
		auto& element = iterator->second;
		if (auto iterator = _table.find(element); iterator != _table.end())
		{
			_vector.push_back(std::move(element));
			_table.erase(iterator);
		}
	}
	return _vector.size() > size;
}

template <typename _TimeType, typename _Element>
bool TimeoutQueue<_TimeType, _Element>::pop(Vector& _vector)
{
	if (empty()) return false;

	_vector.reserve(_vector.size() + size());
	for (auto& [_, element] : _queue)
		_vector.push_back(std::move(element));

	clear();
	return true;
}
