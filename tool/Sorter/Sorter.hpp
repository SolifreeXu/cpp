#pragma once

#include <utility>
#include <cstddef>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <mutex>
#include <shared_mutex>

template <typename _IDType, typename _RecordType, typename _SizeType = size_t>
class Sorter
{
public:
	using IDType = _IDType;
	using RecordType = _RecordType;
	using SizeType = _SizeType;
	using RecordList = std::vector<RecordType>;

private:
	struct node_type
	{
		std::shared_ptr<RecordType> _record;

		node_type() = default;
		node_type(const RecordType& _record) noexcept
			: _record(std::make_shared<RecordType>(_record)) {}

		bool operator<(const node_type& _another) const noexcept
		{
			return *this->_record < *_another._record;
		}
	};

	using NodeType = node_type;
	using MappingType = std::pair<IDType, NodeType>;

private:
	std::unordered_map<IDType, NodeType> _mappingTable;
	std::set<NodeType> _recordList;
	mutable std::shared_mutex _mutex;

public:
	// 获取记录数量
	auto size() const
	{
		std::shared_lock lock(_mutex);
		return _recordList.size();
	}

	/*
	 * 获取指定ID的原始记录
	 * 若无指定ID的记录，则返回空智能指针。
	 */
	std::shared_ptr<const RecordType> find(IDType _id) const
	{
		std::shared_lock lock(_mutex);
		auto iterator = _mappingTable.find(_id);
		return iterator != _mappingTable.end() ? iterator->second._record : nullptr;
	}

	/*
	 * 获取指定起始位置的指定数量的记录列表
	 * 返回指向记录列表的共享指针，避免过多复制对象和拷贝内存，易于共享访问向量。
	 * 若指定数量为零，则返回所有记录。
	 */
	std::shared_ptr<RecordList> get(SizeType _index = 0, SizeType _number = 0) const
	{
		std::shared_lock lock(_mutex);
		if (_index >= _recordList.size())
			return nullptr;

		if (_number <= 0)
			_number = _recordList.size() - _index;
		else
			_number = std::min(_recordList.size() - _index, _number);

		auto recordList = std::make_shared<RecordList>(0);
		recordList->reserve(_number);

		auto iterator = _recordList.crbegin();
		for (; _index > 0; --_index, ++iterator);

		for (; _number > 0; --_number)
			recordList->push_back(*(*iterator++)._record);
		return recordList;
	}

	/*
	 * 新增或者更新记录
	 * 倘若无指定记录，直接插入作为新记录，否则先删除旧记录，再插入新记录。
	 */
	void update(IDType _id, const RecordType& _record)
	{
		std::lock_guard lock(_mutex);
		auto iterator = _mappingTable.find(_id);
		if (iterator == _mappingTable.end())
		{
			NodeType node = _record;
			_mappingTable[_id] = node;
			_recordList.insert(node);
			return;
		}

		NodeType& node = iterator->second;
		_recordList.erase(node);
		*node._record = _record;
		_recordList.insert(node);
	}

	// 移除单条记录，返回移除数量
	SizeType remove(IDType _id)
	{
		std::lock_guard lock(_mutex);
		auto iterator = _mappingTable.find(_id);
		if (iterator == _mappingTable.end())
			return 0;

		_recordList.erase(iterator->second);
		return _mappingTable.erase(iterator);
	}

	// 清空数据
	void clear()
	{
		std::lock_guard lock(_mutex);
		_mappingTable.clear();
		_recordList.clear();
	}

	/*
	 * 备份数据
	 * 复制所有记录，拷贝内存数据。
	 * 返回指向备份排序者的共享指针，易于共享访问排序者。
	 */
	std::shared_ptr<Sorter> backup() const
	{
		std::shared_lock lock(_mutex);
		auto sorter = std::make_shared<Sorter>();
		sorter->_mappingTable.rehash(this->_mappingTable.bucket_count());

		std::transform(this->_mappingTable.cbegin(), this->_mappingTable.cend(),
			std::inserter(sorter->_mappingTable, sorter->_mappingTable.begin()),
			[&sorter](const MappingType& _pair)
			{
				NodeType node = *_pair.second._record;
				sorter->_recordList.insert(node);
				return std::make_pair(_pair.first, node);
			});
		return sorter;
	}
};
