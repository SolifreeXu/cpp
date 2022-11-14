﻿#pragma once

#include <utility>
#include <vector>
#include <set>
#include <unordered_map>
#include <iterator>
#include <algorithm>

template <typename _IDType, typename _RecordType>
class Sorter
{
public:
	using IDType = _IDType;
	using RecordType = _RecordType;

private:
	using PairType = std::pair<IDType, RecordType>;
	using RecordSet = std::set<RecordType>;

public:
	using SizeType = RecordSet::size_type;
	using RecordList = std::vector<RecordType>;

private:
	std::unordered_map<IDType, RecordType> _mapping;
	RecordSet _recordSet;

private:
	/*
	 * 复制数据
	 * 复制所有记录，拷贝内存数据。
	 */
	void copy(const Sorter& _another);

	/*
	 * 根据指定方向遍历，获取指定ID的排名
	 * 若无指定ID，则返回0。
	 */
	template <typename _Iterator>
	SizeType rank(IDType _id, _Iterator _iterator) const;

	/*
	 * 从指定位置起，向指定方向遍历，获取指定数量的记录
	 * 倘若指定数量为零，默认获取所有记录，否则获取指定数量的记录。
	 */
	template <typename _Iterator>
	RecordList& get(RecordList& _recordList, SizeType _index, \
		SizeType _size, _Iterator _iterator) const;

public:
	Sorter() = default;

	Sorter(const Sorter& _another)
		: _mapping(_another._mapping.bucket_count())
	{
		copy(_another);
	}

	Sorter(Sorter&&) = default;

	Sorter& operator=(const Sorter& _another);

	Sorter& operator=(Sorter&&) = default;

	// 是否为空
	bool empty() const noexcept
	{
		return _recordSet.empty();
	}

	// 获取记录数量
	auto size() const noexcept
	{
		return _recordSet.size();
	}

	// 是否存在指定记录
	bool exist(IDType _id) const
	{
		return _mapping.contains(_id);
	}

	// 查找指定ID的原始记录
	const RecordType* find(IDType _id) const
	{
		auto iterator = _mapping.find(_id);
		return iterator != _mapping.end() ? \
			&iterator->second : nullptr;
	}

	/*
	 * 新增或者更新记录
	 * 倘若无指定记录，直接插入作为新记录，否则先删除旧记录，再插入新记录。
	 */
	void update(const RecordType& _record);

	// 移除单条记录，返回移除结果
	bool remove(IDType _id);

	// 清空数据
	void clear() noexcept
	{
		_mapping.clear();
		_recordSet.clear();
	}

	// 获取首记录
	const RecordType* front(bool _forward = false) const noexcept
	{
		if (empty()) return nullptr;
		return _forward ? \
			&*_recordSet.cbegin() : &*_recordSet.crbegin();
	}

	// 获取尾记录
	auto back(bool _forward = false) const noexcept
	{
		return front(not _forward);
	}

	/*
	 * 根据指定方向遍历，获取指定ID的排名
	 * 若无指定ID，则返回0。
	 */
	auto rank(IDType _id, bool _forward = false) const noexcept
	{
		return _forward ? \
			rank(_id, _recordSet.cbegin()) : rank(_id, _recordSet.crbegin());
	}

	/*
	 * 从指定位置起，向指定方向遍历，获取指定数量的记录
	 * 倘若指定数量为零，默认获取所有记录，否则获取指定数量的记录。
	 */
	RecordList& get(RecordList& _recordList, SizeType _index = 0, \
		SizeType _size = 0, bool _forward = false) const;
};

// 复制数据
template <typename _IDType, typename _RecordType>
void Sorter<_IDType, _RecordType>::copy(const Sorter& _another)
{
	std::transform(_another._mapping.cbegin(), _another._mapping.cend(), \
		std::inserter(this->_mapping, this->_mapping.begin()), \
		[this](const PairType& _pair)
		{
			this->_recordSet.insert(_pair.second);
			return _pair;
		});
}

// 获取指定ID的排名
template <typename _IDType, typename _RecordType>
template <typename _Iterator>
auto Sorter<_IDType, _RecordType>::rank(IDType _id, \
	_Iterator _iterator) const -> SizeType
{
	SizeType ranking = 0;
	for (decltype(ranking) index = 0; \
		index < _recordSet.size(); \
		++index, ++_iterator)
		if (IDType(*_iterator) == _id)
		{
			ranking = index + 1;
			break;
		}
	return ranking;
}

// 获取指定数量的记录
template <typename _IDType, typename _RecordType>
template <typename _Iterator>
auto Sorter<_IDType, _RecordType>::get(RecordList& _recordList, \
	SizeType _index, SizeType _size, _Iterator _iterator) const -> RecordList&
{
	for (; _index > 0; --_index, ++_iterator);
	for (; _size > 0; --_size)
		_recordList.push_back(*_iterator++);
	return _recordList;
}

template <typename _IDType, typename _RecordType>
auto Sorter<_IDType, _RecordType>::operator=(const Sorter& _another) -> Sorter&
{
	clear();
	this->_mapping.rehash(_another._mapping.bucket_count());
	copy(_another);
	return *this;
}

// 新增或者更新记录
template <typename _IDType, typename _RecordType>
void Sorter<_IDType, _RecordType>::update(const RecordType& _record)
{
	auto id = IDType(_record);
	auto iterator = _mapping.find(id);
	if (iterator == _mapping.end())
	{
		_mapping[id] = _record;
		_recordSet.insert(_record);
		return;
	}

	_recordSet.erase(iterator->second);
	iterator->second = _record;
	_recordSet.insert(_record);
}

// 移除单条记录
template <typename _IDType, typename _RecordType>
bool Sorter<_IDType, _RecordType>::remove(IDType _id)
{
	auto iterator = _mapping.find(_id);
	if (iterator == _mapping.end())
		return false;

	_recordSet.erase(iterator->second);
	_mapping.erase(iterator);
	return true;
}

// 获取指定数量的记录
template <typename _IDType, typename _RecordType>
auto Sorter<_IDType, _RecordType>::get(RecordList& _recordList, \
	SizeType _index, SizeType _size, bool _forward) const -> RecordList&
{
	if (_index >= _recordSet.size()) return _recordList;

	if (_size <= 0)
		_size = _recordSet.size() - _index;
	else
		_size = std::min(_recordSet.size() - _index, _size);

	_recordList.reserve(_recordList.size() + _size);

	if (_forward)
		return get(_recordList, _index, _size, _recordSet.cbegin());
	else
		return get(_recordList, _index, _size, _recordSet.crbegin());
}
