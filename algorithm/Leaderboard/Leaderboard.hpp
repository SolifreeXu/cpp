#pragma once

#include <utility>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <set>
#include <unordered_map>

class Leaderboard
{
	struct data_type
	{
		uint_least64_t id;
		uint_least64_t integral;
		uint_least64_t timestamp;
	};

	struct node_type
	{
		std::shared_ptr<data_type> data;

		node_type() {}
		node_type(data_type&& data)
			: data(std::make_shared<data_type>(std::move(data))) {}

		bool operator <(const node_type& another) const
		{
			const data_type& another_data = *another.data;
			return data->integral < another_data.integral
				|| data->integral == another_data.integral
				&& (data->timestamp > another_data.timestamp
					|| data->timestamp == another_data.timestamp
					&& data->id > another_data.id);
		}
	};

	std::unordered_map<uint_least64_t, node_type> mapping_table;
	std::set<node_type> ranking_list;
public:
	auto get(size_t number) const
	{
		number = std::min(ranking_list.size(), number);
		if (number <= 0)
			return std::vector<node_type>(0);

		std::vector<node_type> nodes(0);
		nodes.reserve(number);
		auto iterator = ranking_list.crbegin();
		for (; number > 0; --number)
			nodes.push_back(*iterator++);
		return nodes;
	}

	void update(uint_least64_t id, uint_least64_t integral, uint_least64_t timestamp)
	{
		auto iterator = mapping_table.find(id);
		if (iterator == mapping_table.end())
		{
			node_type node = node_type(data_type{ id, integral, timestamp });
			mapping_table[id] = node;
			ranking_list.insert(node);
			return;
		}

		node_type& node = iterator->second;
		ranking_list.erase(node);
		node.data->integral = integral;
		node.data->timestamp = timestamp;
		ranking_list.insert(node);
	}
};
