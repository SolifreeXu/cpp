#include "LRUQueue.hpp"

#include <functional>
#include <iostream>

struct Key
{
	int _integer;

	Key(int _integer) : _integer(_integer) {}

	friend bool operator==(const Key& _left, const Key& _right) noexcept
	{
		return _left._integer == _right._integer;
	}
};

namespace std
{
	template <>
	struct hash<Key>
	{
	public:
		size_t operator()(const Key& _key) const noexcept
		{
			return hash<decltype(_key._integer)>()(_key._integer);
		}
	};
}

int main()
{
	LRUQueue<Key, int> queue(9);
	for (int index = 0; index < 10; ++index)
		queue.push(index, index);

	if (auto result = queue.find(9))
		std::cout << result.value() << std::endl;

	queue.pop(9);
	queue.clear();
	return 0;
}
