#include "LRUQueue.hpp"
#include "Version.hpp"

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

#if CXX_VERSION >= CXX_2017
	using std::cout, std::endl;
#else
	using std::cout;
	using std::endl;
#endif

	CacheType cache(9);
	auto index = 0;
	for (; index < 10; ++index)
		cache.push(index, index);

#if CXX_VERSION >= CXX_2017
	if (auto result = cache.find(1); \
		result != nullptr)
#else
	auto result = cache.find(1);
	if (result != nullptr)
#endif
		cout << *result << endl;

	cout << cache.size() << endl;
	cache.pop(--index);
	cout << cache.size() << endl;

#if CXX_VERSION >= CXX_2017
	if (QueueType queue; cache.pop(queue))
#else
	QueueType queue;
	if (cache.pop(queue))
#endif
	{
		for (const auto& pair : queue)
			cout << pair.second << ' ';
		cout << '\b' << endl;
	}

	cout << std::boolalpha \
		<< cache.empty() << endl;
	return EXIT_SUCCESS;
}
