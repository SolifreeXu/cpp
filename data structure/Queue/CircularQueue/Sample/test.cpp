#include "CircularQueue.hpp"
#include "Integer.hpp"

#include <cstdlib>
#include <utility>
#include <algorithm>
#include <iostream>

template <typename _Element>
static void print(const CircularQueue<_Element>& _queue)
{
	std::cout << _queue.size() \
		<< '/' << _queue.capacity() \
		<< '/' << _queue.max_size() << std::endl;
}

template <typename _Element>
static void traversePrint(const CircularQueue<_Element>& _queue, \
	bool _indexable = false)
{
	if (_queue.empty()) return;

	print(_queue);

	if (_indexable)
		for (decltype(_queue.size()) index = 0; \
			index < _queue.size(); ++index)
			std::cout << _queue[index] << ' ';
	else
		for (auto& element : _queue)
			std::cout << element << ' ';

	std::cout << '\n' << std::endl;
}

template <typename _Element>
static void reversePrint(const CircularQueue<_Element>& _queue)
{
	if (_queue.empty()) return;

	print(_queue);

	std::for_each(_queue.crbegin(), _queue.crend(), \
		[](const _Element& element)
		{
			std::cout << element << ' ';
		});

	std::cout << '\n' << std::endl;
}

int main()
{
	//using QueueType = CircularQueue<int>;
	using QueueType = CircularQueue<Integer>;
	using value_type = QueueType::value_type;

	using std::cout;
	using std::endl;

	QueueType queue1;
	for (int index = 1; index < 5; ++index)
		queue1.push_back(index);

	for (int index = 5; index < 10; ++index)
		queue1.emplace_back(index);

	for (int index = 1; index < 5; ++index)
		queue1.push_front(index);

	for (int index = 5; index < 10; ++index)
		queue1.emplace_front(index);

	traversePrint(queue1, true);

	queue1.pop_back();
	queue1.pop_front();
	traversePrint(queue1);

	queue1.shrink_to_fit();
	traversePrint(queue1);

	auto queue2 = queue1;
	traversePrint(queue2);

	queue2.clear();
	print(queue2);
	cout << std::boolalpha << queue2.empty() << '\n' << endl;

	for (int index = 1; index < 5; ++index)
		queue2.insert(queue2.begin(), index);

	// 特殊用法：相对位置
	auto iterator = queue2.cend();
	for (int index = 9; index > 5; --index)
		queue2.insert(iterator, index);

	for (int index = 5; index > 0; --index)
		queue2.emplace(iterator, index);

	for (int index = 5; index < 10; ++index)
		queue2.emplace(queue2.begin(), index);

	traversePrint(queue2);

	if (auto iterator = std::find(queue2.crbegin(), \
		queue2.crend(), 1); iterator != queue2.crend())
		queue2.insert(--iterator.base(), 0);
	reversePrint(queue2);

	QueueType queue3(queue2.cbegin(), queue2.cend());
	print(queue3);
	cout << queue3.front() << ' ' \
		<< queue3.at(9) << ' ' \
		<< queue3.back() << '\n' << endl;

	queue3.erase(queue3.cbegin());
	queue3.erase(--queue3.cend());
	traversePrint(queue3);

	cout << std::erase(queue3, 8) << endl;
	traversePrint(queue3);

	auto size = std::erase_if(queue3, \
		[](const value_type& _value)
		{
			return _value == 4;
		});
	cout << size << endl;
	traversePrint(queue3);

	iterator = std::find(queue3.cbegin(), queue3.cend(), 0);
	if (iterator != queue3.cend())
	{
		auto next = queue3.erase(queue3.cbegin(), iterator);
		cout << *next << endl;
	}
	traversePrint(queue3);

	iterator = std::find(queue3.cbegin(), queue3.cend(), 0);
	if (iterator != queue3.cend())
	{
		auto next = queue3.erase(++iterator, queue3.cend());
		cout << std::boolalpha << (next == queue3.end()) << endl;
	}
	traversePrint(queue3);

	queue3.swap(queue1);
	traversePrint(queue3);

	iterator = std::find(queue3.cbegin(), queue3.cend(), 1);
	if (iterator != queue3.cend())
		cout << *queue3.erase(iterator) << endl;
	traversePrint(queue3);

	auto first = std::find(queue3.cbegin(), queue3.cend(), 2);
	auto last = std::find(queue3.crbegin(), queue3.crend(), 2);
	if (first != queue3.cend() and last != queue3.crend())
		cout << *queue3.erase(first, last.base()) << endl;
	traversePrint(queue3);

	queue3.insert(first, { 2, 1, 0, 1, 2 });
	traversePrint(queue3);

	queue3.resize(3);
	traversePrint(queue3);

	queue3 = std::move(queue1);
	traversePrint(queue3);

	queue3.resize(9, 0);
	traversePrint(queue3);
	return EXIT_SUCCESS;
}
