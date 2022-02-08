/*
* 文件名称：TimeoutQueue.hpp
* 摘要：
* 1.超时队列按照时间对元素排序，提供放入、取出、清空等方法。
*   支持根据超时与索引移除元素，以及指定时间批量取出超时元素。
*
* 版本：v1.0.0
* 作者：许聪
* 邮箱：2592419242@qq.com
* 创建日期：2022年01月28日
* 更新日期：2022年02月08日
*/

#pragma once

#include <utility>
#include <optional>
#include <vector>
#include <map>

template <typename _IndexType, typename _ElementType, typename _TimeType>
class TimeoutQueue
{
public:
	using IndexType = _IndexType;
	using ElementType = _ElementType;
	using TimeType = _TimeType;

private:
	using QueueType = std::multimap<TimeType, IndexType>;
	using PoolType = std::map<IndexType, std::pair<ElementType, TimeType>>;

public:
	using SizeType = typename QueueType::size_type;
	using PairType = std::pair<IndexType, ElementType>;
	using VectorType = std::vector<PairType>;

private:
	SizeType _capacity;
	QueueType _queue;
	PoolType _pool;

private:
	void erase(TimeType _time, const IndexType& _index);

public:
	TimeoutQueue(SizeType _capacity = 0) : _capacity(_capacity) {}

	auto capacity() const noexcept { return _capacity; }
	void reserve(SizeType _capacity) noexcept { this->_capacity = _capacity; }

	auto size() const noexcept { return _queue.size(); }
	bool empty() const noexcept { return _queue.empty(); }

	bool push(const IndexType& _index, const ElementType& _element, TimeType _time);

	std::optional<ElementType> pop(const IndexType& _index);
	bool pop(TimeType _time, VectorType& _vector);

	void clear() noexcept
	{
		_queue.clear();
		_pool.clear();
	}
};

template <typename _IndexType, typename _ElementType, typename _TimeType>
void TimeoutQueue<_IndexType, _ElementType, _TimeType>::erase(TimeType _time, const IndexType& _index)
{
	auto iterator = _queue.lower_bound(_time);
	if (iterator == _queue.end())
		return;

	auto end = _queue.upper_bound(_time);
	for (; iterator != end; ++iterator)
		if (iterator->second == _index)
		{
			_queue.erase(iterator);
			break;
		}
}

template <typename _IndexType, typename _ElementType, typename _TimeType>
bool TimeoutQueue<_IndexType, _ElementType, _TimeType>::push(const IndexType& _index, const ElementType& _element, TimeType _time)
{
	if (_capacity > 0 && _queue.size() >= _capacity)
		return false;

	_queue.emplace(_time, _index);
	_pool.emplace(_index, std::make_pair(_element, _time));
	return true;
}

template <typename _IndexType, typename _ElementType, typename _TimeType>
auto TimeoutQueue<_IndexType, _ElementType, _TimeType>::pop(const IndexType& _index) -> std::optional<ElementType>
{
	auto iterator = _pool.find(_index);
	if (iterator == _pool.end())
		return std::nullopt;

	auto& [element, time] = iterator->second;
	std::optional result = element;

	erase(time, _index);
	_pool.erase(iterator);
	return result;
}

template <typename _IndexType, typename _ElementType, typename _TimeType>
bool TimeoutQueue<_IndexType, _ElementType, _TimeType>::pop(TimeType _time, VectorType& _vector)
{
	auto iterator = _queue.lower_bound(_time);
	if (iterator == _queue.end())
		return false;

	bool result = false;
	for (auto end = _queue.upper_bound(_time); \
		iterator != end; iterator = _queue.erase(iterator))
	{
		const auto& index = iterator->second;
		if (auto iterator = _pool.find(index); iterator != _pool.end())
		{
			_vector.emplace_back(iterator->first, iterator->second.first);
			_pool.erase(iterator);
			result = true;
		}
	}
	return result;
}
