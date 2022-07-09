#pragma once

#include <utility>
#include <memory>
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
	struct Node
	{
		IDType _id;
		std::shared_ptr<RecordType> _record;

		Node() noexcept = default;
		Node(IDType _id, const RecordType& _record)
			: _id(_id), _record(std::make_shared<RecordType>(_record)) {}

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
	void copy(const Sorter& _another)
	{
		std::transform(_another._mapping.cbegin(), _another._mapping.cend(), \
			std::inserter(this->_mapping, this->_mapping.begin()), \
			[this](const PairType& _pair)
			{
				NodeType node(_pair.first, *_pair.second._record);
				this->_nodeSet.insert(node);
				return std::make_pair(_pair.first, node);
			});
	}

public:
	Sorter() = default;

	Sorter(const Sorter& _another)
		: _mapping(_another._mapping.bucket_count())
	{
		copy(_another);
	}

	Sorter(Sorter&&) = default;

	Sorter& operator=(const Sorter& _another)
	{
		clear();
		this->_mapping.rehash(_another._mapping.bucket_count());
		copy(_another);
		return *this;
	}

	Sorter& operator=(Sorter&&) = default;

	// 获取记录数量
	auto size() const noexcept
	{
		return _nodeSet.size();
	}

	/*
	 * 查找指定ID的原始记录
	 * 若无指定ID的记录，则返回空智能指针。
	 */
	std::shared_ptr<const RecordType> find(IDType _id) const
	{
		auto iterator = _mapping.find(_id);
		return iterator != _mapping.end() ? \
			iterator->second._record : nullptr;
	}

	/*
	 * 从指定位置起，获取指定数量的记录
	 * 返回指向记录列表的共享指针，尽量避免复制对象和拷贝内存，并且易于共享访问向量。
	 */
	std::shared_ptr<RecordList> get(SizeType _index = 0, \
		SizeType _size = 0) const
	{
		if (_index >= _nodeSet.size()) return nullptr;

		if (_size <= 0)
			_size = _nodeSet.size() - _index;
		else
			_size = std::min(_nodeSet.size() - _index, _size);

		auto recordList = std::make_shared<RecordList>(0);
		recordList->reserve(_size);

		auto iterator = _nodeSet.crbegin();
		for (; _index > 0; --_index, ++iterator);

		for (; _size > 0; --_size)
			recordList->push_back(*(*iterator++)._record);
		return recordList;
	}

	/*
	 * 从指定位置起，获取指定数量的记录
	 * 从指定位置向后遍历，倘若指定数量为零，默认获取所有记录，否则获取指定数量的记录。
	 */
	RecordList& get(RecordList& _recordList, SizeType _index = 0, \
		SizeType _size = 0) const
	{
		if (_index >= _nodeSet.size()) return _recordList;

		if (_size <= 0)
			_size = _nodeSet.size() - _index;
		else
			_size = std::min(_nodeSet.size() - _index, _size);

		_recordList.reserve(_recordList.size() + _size);

		auto iterator = _nodeSet.crbegin();
		for (; _index > 0; --_index, ++iterator);

		for (; _size > 0; --_size)
			_recordList.push_back(*(*iterator++)._record);
		return _recordList;
	}

	/*
	 * 获取指定ID的排名
	 * 若无指定ID，则返回0。
	 */
	auto rank(IDType _id) const noexcept
	{
		SizeType ranking = 0;
		auto iterator = _nodeSet.crbegin();
		for (decltype(ranking) index = 0; \
			index < _nodeSet.size(); ++index, ++iterator)
			if (iterator->_id == _id)
			{
				ranking = index + 1;
				break;
			}
		return ranking;
	}

	/*
	 * 新增或者更新记录
	 * 倘若无指定记录，直接插入作为新记录，否则先删除旧记录，再插入新记录。
	 */
	void update(IDType _id, const RecordType& _record)
	{
		auto iterator = _mapping.find(_id);
		if (iterator == _mapping.end())
		{
			NodeType node(_id, _record);
			_mapping[_id] = node;
			_nodeSet.insert(node);
			return;
		}

		NodeType& node = iterator->second;
		_nodeSet.erase(node);
		*node._record = _record;
		_nodeSet.insert(node);
	}

	// 移除单条记录，返回移除数量
	SizeType remove(IDType _id)
	{
		auto iterator = _mapping.find(_id);
		if (iterator == _mapping.end())
			return 0;

		_nodeSet.erase(iterator->second);
		_mapping.erase(iterator);
		return 1;
	}

	// 清空数据
	void clear() noexcept
	{
		_mapping.clear();
		_nodeSet.clear();
	}
};
