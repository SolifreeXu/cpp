#include "TimeoutQueue.hpp"

#include <cstdlib>
#include <type_traits>
#include <functional>
#include <ctime>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
	constexpr int array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	using ElementType = std::remove_const_t<std::remove_reference_t<decltype(array[0])>>;
	using FunctorType = std::function<const ElementType* ()>;
	using QueueType = TimeoutQueue<const ElementType*, FunctorType, std::time_t>;

	QueueType queue;
	for (auto& element : array)
	{
		auto address = &element;
		auto time = std::time(nullptr);
		queue.push(address, \
			[address] { return address; }, time);
	}
	std::cout << queue.size() << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(1));

	auto address = &array[0];
	if (auto pair = queue.find(address); pair != nullptr)
		std::cout << address << ' ' \
			<< *pair->first() << std::endl;

	queue.pop(address);
	std::cout << queue.size() << std::endl;

	QueueType::VectorType vector;
	if (queue.pop(std::time(nullptr), vector))
		for (const auto& [address, functor] : vector)
			std::cout << address << ' ' \
				<< *functor() << std::endl;

	std::cout << std::boolalpha \
		<< queue.empty() << std::endl;
	return EXIT_SUCCESS;
}
