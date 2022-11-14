#pragma once

#include <utility>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include <iterator>
#include <algorithm>

template <typename _IDType, typename _RecordType>
class SharedSorter
{
public:
	using IDType = _IDType;
	using RecordType = _RecordType;

private:
	struct Node
	{
		std::shared_ptr<RecordType> _record;

		Node() noexcept = default;
		Node(const RecordType& _record)
			: _record(std::make_shared<RecordType>(_record)) {}

		bool operator<(const Node& _another) const noexcept
		{
			return *this->_record < *_another._record;
		}
	};

private:
	using NodeType = Node;
	using PairType = std::pair<IDType, NodeType>;
	using NodeSet = std::set<NodeType>;

public:
	using SizeType = NodeSet::size_type;
	using RecordList = std::vector<RecordType>;

private:
	std::unordered_map<IDType, NodeType> _mapping;
	NodeSet _nodeSet;

private:
	/*
	 * 复制数据
	 * 复制所有记录，拷贝内存数据。
	 */
	void copy(const SharedSorter& _another);

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
	SharedSorter() = default;

	SharedSorter(const SharedSorter& _another)
		: _mapping(_another._mapping.bucket_count())
	{
		copy(_another);
	}

	SharedSorter(SharedSorter&&) = default;

	SharedSorter& operator=(const SharedSorter& _another);

	SharedSorter& operator=(SharedSorter&&) = default;

	// 是否为空
	bool empty() const noexcept
	{
		return _nodeSet.empty();
	}

	// 获取记录数量
	auto size() const noexcept
	{
		return _nodeSet.size();
	}

	// 是否存在指定记录
	bool exist(IDType _id) const
	{
		return _mapping.contains(_id);
	}

	// 查找指定ID的原始记录
	std::shared_ptr<const RecordType> find(IDType _id) const
	{
		auto iterator = _mapping.find(_id);
		return iterator != _mapping.end() ? \
			iterator->second._record : nullptr;
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
		_nodeSet.clear();
	}

	// 获取首记录
	std::shared_ptr<const RecordType> front(bool _forward = false) const noexcept
	{
		if (empty()) return nullptr;
		return _forward ? \
			_nodeSet.cbegin()->_record : _nodeSet.crbegin()->_record;
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
			rank(_id, _nodeSet.cbegin()) : rank(_id, _nodeSet.crbegin());
	}

	/*
	 * 从指定位置起，向指定方向遍历，获取指定数量的记录
	 * 返回记录列表的共享指针，避免复制对象和拷贝内存，并且易于共享访问向量。
	 */
	std::shared_ptr<RecordList> get(SizeType _index = 0, \
		SizeType _size = 0, bool _forward = false) const;

	/*
	 * 从指定位置起，向指定方向遍历，获取指定数量的记录
	 * 倘若指定数量为零，默认获取所有记录，否则获取指定数量的记录。
	 */
	RecordList& get(RecordList& _recordList, SizeType _index = 0, \
		SizeType _size = 0, bool _forward = false) const;
};

// 复制数据
template <typename _IDType, typename _RecordType>
void SharedSorter<_IDType, _RecordType>::copy(const SharedSorter& _another)
{
	std::transform(_another._mapping.cbegin(), _another._mapping.cend(), \
		std::inserter(this->_mapping, this->_mapping.begin()), \
		[this](const PairType& _pair)
		{
			NodeType node(*_pair.second._record);
			this->_nodeSet.insert(node);
			return std::make_pair(_pair.first, node);
		});
}

// 获取指定ID的排名
template <typename _IDType, typename _RecordType>
template <typename _Iterator>
auto SharedSorter<_IDType, _RecordType>::rank(IDType _id, \
	_Iterator _iterator) const -> SizeType
{
	SizeType ranking = 0;
	for (decltype(ranking) index = 0; \
		index < _nodeSet.size(); ++index, ++_iterator)
		if (IDType(*_iterator->_record) == _id)
		{
			ranking = index + 1;
			break;
		}
	return ranking;
}

// 获取指定数量的记录
template <typename _IDType, typename _RecordType>
template <typename _Iterator>
auto SharedSorter<_IDType, _RecordType>::get(RecordList& _recordList, \
	SizeType _index, SizeType _size, _Iterator _iterator) const -> RecordList&
{
	for (; _index > 0; --_index, ++_iterator);
	for (; _size > 0; --_size)
		_recordList.push_back(*(*_iterator++)._record);
	return _recordList;
}

template <typename _IDType, typename _RecordType>
auto SharedSorter<_IDType, _RecordType>::operator=(const SharedSorter& _another) -> SharedSorter&
{
	clear();
	this->_mapping.rehash(_another._mapping.bucket_count());
	copy(_another);
	return *this;
}

// 新增或者更新记录
template <typename _IDType, typename _RecordType>
void SharedSorter<_IDType, _RecordType>::update(const RecordType& _record)
{
	auto id = IDType(_record);
	auto iterator = _mapping.find(id);
	if (iterator == _mapping.end())
	{
		NodeType node(_record);
		_mapping[id] = node;
		_nodeSet.insert(node);
		return;
	}

	NodeType& node = iterator->second;
	_nodeSet.erase(node);
	*node._record = _record;
	_nodeSet.insert(node);
}

// 移除单条记录
template <typename _IDType, typename _RecordType>
bool SharedSorter<_IDType, _RecordType>::remove(IDType _id)
{
	auto iterator = _mapping.find(_id);
	if (iterator == _mapping.end())
		return false;

	_nodeSet.erase(iterator->second);
	_mapping.erase(iterator);
	return true;
}

// 获取指定数量的记录
template <typename _IDType, typename _RecordType>
auto SharedSorter<_IDType, _RecordType>::get(SizeType _index, \
	SizeType _size, bool _forward) const -> std::shared_ptr<RecordList>
{
	if (_index >= _nodeSet.size()) return nullptr;

	if (_size <= 0)
		_size = _nodeSet.size() - _index;
	else
		_size = std::min(_nodeSet.size() - _index, _size);

	auto recordList = std::make_shared<RecordList>(0);
	recordList->reserve(_size);

	if (_forward)
		get(*recordList, _index, _size, _nodeSet.cbegin());
	else
		get(*recordList, _index, _size, _nodeSet.crbegin());
	return recordList;
}

// 获取指定数量的记录
template <typename _IDType, typename _RecordType>
auto SharedSorter<_IDType, _RecordType>::get(RecordList& _recordList, \
	SizeType _index, SizeType _size, bool _forward) const -> RecordList&
{
	if (_index >= _nodeSet.size()) return _recordList;

	if (_size <= 0)
		_size = _nodeSet.size() - _index;
	else
		_size = std::min(_nodeSet.size() - _index, _size);

	_recordList.reserve(_recordList.size() + _size);

	if (_forward)
		return get(_recordList, _index, _size, _nodeSet.cbegin());
	else
		return get(_recordList, _index, _size, _nodeSet.crbegin());
}
