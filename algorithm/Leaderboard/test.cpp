#include <iostream>

#include "Leaderboard.hpp"

int main()
{
	Leaderboard leaderboard;
	for (size_t index = 0; index < 100; ++index)
		leaderboard.update(index + 1, 100 - index, index + 1);
	leaderboard.update(10, 100, 101);
	leaderboard.update(100, 100, 101);
	auto nodes = leaderboard.get(10);
	for (auto& node : nodes)
		std::cout << node.data->id << '\t' << node.data->integral << '\t' << node.data->timestamp << std::endl;
	return 0;
}
