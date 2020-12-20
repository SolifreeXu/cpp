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

template <typename id_type, typename data_type, typename size_type = size_t>
class Leaderboard
{
	struct node_type
	{
		std::shared_ptr<data_type> data;

		node_type() = default;
		node_type(const data_type& data) noexcept
			: data(std::make_shared<data_type>(data)) {}

		bool operator<(const node_type& another) const noexcept
		{
			return *this->data < *another.data;
		}
	};

	std::unordered_map<id_type, node_type> mapping_table;
	std::set<node_type> ranking_list;
	std::shared_mutex mutex;
private:
	using mapping_type = std::pair<id_type, node_type>;
public:
	using datas_type = std::vector<data_type>;

	std::shared_ptr<datas_type> get(size_type number = 0)
	{
		std::shared_lock lock(mutex);
		if (number <= 0)
			number = ranking_list.size();
		else
			number = std::min(ranking_list.size(), number);

		auto records = std::make_shared<datas_type>(0);
		records->reserve(number);
		auto iterator = ranking_list.crbegin();
		for (; number > 0; --number)
			records->push_back(*(*iterator++).data);
		return records;
	}

	std::shared_ptr<Leaderboard> backup()
	{
		std::shared_lock lock(mutex);
		auto leaderboard = std::make_shared<Leaderboard>();
		leaderboard->mapping_table.rehash(this->mapping_table.bucket_count());
		std::transform(this->mapping_table.cbegin(), this->mapping_table.cend(),
			std::inserter(leaderboard->mapping_table, leaderboard->mapping_table.begin()),
			[&leaderboard](const mapping_type& pair)
			{
				node_type node = *pair.second.data;
				leaderboard->ranking_list.insert(node);
				return std::make_pair(pair.first, node);
			});
		return leaderboard;
	}

	std::shared_ptr<const data_type> find(id_type id)
	{
		std::shared_lock lock(mutex);
		auto iterator = mapping_table.find(id);
		if (iterator == mapping_table.end())
			return nullptr;
		return iterator->second.data;
	}

	void update(id_type id, const data_type& data)
	{
		std::lock_guard lock(mutex);
		auto iterator = mapping_table.find(id);
		if (iterator == mapping_table.end())
		{
			node_type node = data;
			mapping_table[id] = node;
			ranking_list.insert(node);
			return;
		}

		node_type& node = iterator->second;
		ranking_list.erase(node);
		*node.data = data;
		ranking_list.insert(node);
	}

	void clear()
	{
		std::lock_guard lock(mutex);
		mapping_table.clear();
		ranking_list.clear();
	}
};
