#include "LRUQueue.hpp"

#include <cstdlib>
#include <functional>
#include <iostream>

struct Key
{
	int _integer;

	Key(decltype(_integer) _integer) noexcept
		: _integer(_integer) {}

	friend bool operator==(const Key& _left, \
		const Key& _right) noexcept
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
	using QueueType = LRUQueue<Key, int>;
	QueueType queue(9);
	auto index = 0;
	for (; index < 10; ++index)
		queue.push(index, index);

	if (auto result = queue.find(1))
		std::cout << result.value() << std::endl;

	std::cout << queue.size() << std::endl;
	queue.pop(--index);
	std::cout << queue.size() << std::endl;

	if (QueueType::VectorType vector; \
		queue.pop(vector))
	{
		for (const auto& pair : vector)
			std::cout << pair.second << ' ';
		std::cout << '\b' << std::endl;;
	}

	std::cout << std::boolalpha << queue.empty() << std::endl;
	return EXIT_SUCCESS;
}
