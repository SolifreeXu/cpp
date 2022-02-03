#include "LRUQueue.hpp"

#include <iostream>

int main()
{
	LRUQueue<int, int> queue(9);
	for (int index = 0; index < 10; ++index)
		queue.push(index, index);

	if (auto result = queue.find(9))
		std::cout << result.value() << std::endl;

	queue.pop(9);
	queue.clear();
	return 0;
}
