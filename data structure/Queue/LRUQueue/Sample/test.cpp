#include "LRUQueue.hpp"

#include <cstdlib>
#include <cstddef>
#include <functional>
#include <iostream>

struct Key
{
	int _integer;

	friend bool operator==(const Key& _left, \
		const Key& _right) noexcept
	{
		return _left._integer == _right._integer;
	}

	Key(decltype(_integer) _integer) noexcept : \
		_integer(_integer) {}
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
	using CacheType = LRUQueue<Key, int>;
	using QueueType = CacheType::QueueType;

	CacheType cache(9);
	auto index = 0;
	for (; index < 10; ++index)
		cache.push(index, index);

	if (auto result = cache.find(1))
		std::cout << result.value() << std::endl;

	std::cout << cache.size() << std::endl;
	cache.pop(--index);
	std::cout << cache.size() << std::endl;

	if (QueueType queue; cache.pop(queue))
	{
		for (const auto& pair : queue)
			std::cout << pair.second << ' ';
		std::cout << '\b' << std::endl;;
	}

	std::cout << std::boolalpha \
		<< cache.empty() << std::endl;
	return EXIT_SUCCESS;
}
