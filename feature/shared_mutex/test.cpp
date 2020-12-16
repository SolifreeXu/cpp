#include "shared_mutex.hpp"

#include <chrono>
#include <cstddef>
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

constexpr size_t MAX_CONCURRENCY = 10;

void output(const char* prefix, size_t number)
{
	static std::mutex mutex;
	mutex.lock();
	std::cout << prefix << number << std::endl;
	mutex.unlock();
}

int main()
{
	eterfree::shared_mutex mutex;
	std::atomic_uint exclusive_counter = 0;
	std::atomic_uint shared_counter = 0;
	std::thread threads[MAX_CONCURRENCY];
	for (size_t index = 0; index < MAX_CONCURRENCY; ++index)
	{
		if (index % 5 == 0)
			threads[index] = std::thread([&]()
			{
				std::unique_lock<eterfree::shared_mutex> lock(mutex);
				output("exclusive_counter = ", ++exclusive_counter);
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				--exclusive_counter;
				lock.unlock();
			});
		else
			threads[index] = std::thread([&]()
			{
				eterfree::shared_lock<eterfree::shared_mutex> lock(mutex);
				output("shared_counter = ", ++shared_counter);
				lock.unlock();
				--shared_counter;
			});
	}

	for (size_t index = 0; index < MAX_CONCURRENCY; ++index)
		if (threads[index].joinable())
			threads[index].join();
	return 0;
}
