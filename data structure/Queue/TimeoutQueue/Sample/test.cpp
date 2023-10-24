#include "TimeoutQueue.hpp"

#include <cstdlib>
#include <type_traits>
#include <ctime>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
	using std::cout, std::endl;

	constexpr int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	using Element = std::remove_const_t<std::remove_reference_t<decltype(array[0])>>;
	using QueueType = TimeoutQueue<std::time_t, const Element*>;

	QueueType queue;
	for (auto& element : array)
	{
		auto address = &element;
		auto time = std::time(nullptr);
		queue.push(time, address);
	}
	cout << queue.size() << endl;

	std::this_thread::sleep_for(std::chrono::seconds(1));

	auto address = &array[0];
	queue.pop(address);
	cout << queue.size() << endl;

	QueueType::Vector vector;
	auto time = std::time(nullptr);
	if (queue.pop(time, vector))
		for (auto address : vector)
			cout << address << ' ' \
			<< *address << endl;

	cout << std::boolalpha \
		<< queue.empty() << endl;
	return EXIT_SUCCESS;
}
