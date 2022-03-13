#include "LRUQueue.hpp"

#include <functional>
#include <iostream>

struct Key
{
	int _integer;

	Key(decltype(_integer) _integer) : _integer(_integer) {}

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
	auto index = 0;
	for (; index < 10; ++index)
		queue.push(index, index);

	if (auto result = queue.find(--index))
		std::cout << result.value() << std::endl;

	std::cout << queue.size() << std::endl;
	queue.pop(index);
	std::cout << queue.size() << std::endl;

	queue.clear();
	std::cout << std::boolalpha << queue.empty() << std::endl;
	return 0;
}
