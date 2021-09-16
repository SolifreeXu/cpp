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
	// ��ȡ��¼����
	auto size() const
	{
		std::shared_lock lock(_mutex);
		return _recordList.size();
	}

	/*
	 * ��ȡָ��ID��ԭʼ��¼
	 * ����ָ��ID�ļ�¼���򷵻ؿ�����ָ�롣
	 */
	std::shared_ptr<const RecordType> find(IDType _id) const
	{
		std::shared_lock lock(_mutex);
		auto iterator = _mappingTable.find(_id);
		return iterator != _mappingTable.end() ? iterator->second._record : nullptr;
	}

	/*
	 * ��ȡָ����ʼλ�õ�ָ�������ļ�¼�б�
	 * ����ָ���¼�б�Ĺ���ָ�룬������ิ�ƶ���Ϳ����ڴ棬���ڹ������������
	 * ��ָ������Ϊ�㣬�򷵻����м�¼��
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
	 * �������߸��¼�¼
	 * ������ָ����¼��ֱ�Ӳ�����Ϊ�¼�¼��������ɾ���ɼ�¼���ٲ����¼�¼��
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

	// �Ƴ�������¼�������Ƴ�����
	SizeType remove(IDType _id)
	{
		std::lock_guard lock(_mutex);
		auto iterator = _mappingTable.find(_id);
		if (iterator == _mappingTable.end())
			return 0;

		_recordList.erase(iterator->second);
		return _mappingTable.erase(iterator);
	}

	// �������
	void clear()
	{
		std::lock_guard lock(_mutex);
		_mappingTable.clear();
		_recordList.clear();
	}

	/*
	 * ��������
	 * �������м�¼�������ڴ����ݡ�
	 * ����ָ�򱸷������ߵĹ���ָ�룬���ڹ�����������ߡ�
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
