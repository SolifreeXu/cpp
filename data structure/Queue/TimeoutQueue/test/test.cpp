#include "TimeoutQueue.hpp"

#include <type_traits>
#include <ctime>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
	constexpr int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	using ElementType = std::remove_const_t<std::remove_reference_t<decltype(array[0])>>;
	constexpr auto size = sizeof array / sizeof(ElementType);

	using QueueType = TimeoutQueue<const ElementType*, ElementType, std::time_t>;
	QueueType queue;
	for (const auto& element : array)
	{
		auto time = std::time(nullptr);
		queue.push(&element, element, time);
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << queue.size() << std::endl;
	queue.pop(&array[0]);
	std::cout << queue.size() << std::endl;

	QueueType::VectorType vector;
	if (queue.pop(std::time(nullptr), vector))
		for (const auto& [address, element] : vector)
			std::cout << address << ' ' << element << std::endl;

	std::cout << std::boolalpha << queue.empty() << std::endl;
	return 0;
}
