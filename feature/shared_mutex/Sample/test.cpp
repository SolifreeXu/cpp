#define SHARED_MUTEX

#ifdef SHARED_MUTEX
#include "shared_mutex.hpp"
//#include <shared_mutex>
#endif

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <queue>
#include <iostream>
#include <mutex>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#pragma comment(lib, "WinMM.Lib")
#endif

#ifdef SHARED_MUTEX
//using MutexType = std::shared_mutex;
using MutexType = std::shared_timed_mutex;
#else
using MutexType = std::mutex;
#endif

using TaskType = std::size_t(*)();
using TimePoint = std::chrono::steady_clock::time_point;
using Nanosecond = std::chrono::nanoseconds;

static constexpr auto TOTAL_COUNT = 10000;
static constexpr auto EXCLUSIVE_COUNT = 100;

static const Nanosecond::rep SLEEP_TIME = 1000000;

static MutexType countMutex;
static std::size_t sharedCounter = 0;
static std::size_t exclusiveCounter = 0;

static std::mutex queueMutex;
static std::queue<TaskType> taskQueue;

static TimePoint timePoint;

static void sleepFor(typename Nanosecond::rep _duration)
{
	if (_duration <= 0) return;

#ifdef _WIN32
	constexpr UINT PERIOD = 1;

	::timeBeginPeriod(PERIOD);
#endif

	auto duration = Nanosecond(_duration);
	std::this_thread::sleep_for(duration);

#ifdef _WIN32
	::timeEndPeriod(PERIOD);
#endif
}

static std::size_t read()
{
#ifdef SHARED_MUTEX
	std::shared_lock<MutexType> lock(countMutex);
#else
	std::lock_guard<MutexType> lock(countMutex);
#endif

	sleepFor(SLEEP_TIME);
	++sharedCounter;
	return 0;
}

static std::size_t write()
{
	std::lock_guard<MutexType> lock(countMutex);
	sleepFor(SLEEP_TIME);
	return ++exclusiveCounter;
}

static TaskType getTask()
{
	std::lock_guard<std::mutex> lock(queueMutex);
	if (taskQueue.empty())
		return nullptr;

	auto task = taskQueue.front();
	taskQueue.pop();
	return task;
}

static void execute()
{
	using namespace std::chrono;

	auto task = getTask();
	while (task != nullptr)
	{
		auto count = task();
		if (count == EXCLUSIVE_COUNT)
		{
			auto duration = steady_clock::now() - timePoint;
			auto time = duration_cast<nanoseconds>(duration);
			std::cout << time.count() << std::endl;
		}

		task = getTask();
	}
}

int main()
{
	using namespace std::chrono;

	for (auto index = 0; index < TOTAL_COUNT; ++index)
		if (index % EXCLUSIVE_COUNT == 0)
			taskQueue.push(write);
		else
			taskQueue.push(read);

	auto size = std::thread::hardware_concurrency();
	auto threadPool = new std::thread[size];

	timePoint = steady_clock::now();

	for (decltype(size) index = 0; \
		index < size; ++index)
	{
		auto& thread = threadPool[index];
		thread = std::thread(execute);
	}

#ifdef SHARED_MUTEX
	Nanosecond waitTime(SLEEP_TIME);
	std::size_t sharedCounter = 0, exclusiveCounter = 0;
	for (auto index = 0; index < 1000; ++index)
		if (index % 10 == 0)
		{
			if (countMutex.try_lock())
			{
				countMutex.unlock();

				++exclusiveCounter;
				sleepFor(SLEEP_TIME);
			}
		}
		else if (index % 10 == 5)
		{
			if (index % 20 == 5)
			{
				if (countMutex.try_lock_for(waitTime))
				{
					countMutex.unlock();

					++exclusiveCounter;
				}
			}
			else
			{
				auto timePoint = steady_clock::now();
				timePoint += waitTime;
				if (countMutex.try_lock_until(timePoint))
				{
					countMutex.unlock();

					++exclusiveCounter;
				}
			}
		}
		else if (index % 10 < 5)
		{
			if (countMutex.try_lock_shared())
			{
				countMutex.unlock_shared();

				++sharedCounter;
				sleepFor(SLEEP_TIME);
			}
		}
		else
		{
			if (index % 20 < 10)
			{
				if (countMutex.try_lock_shared_for(waitTime))
				{
					countMutex.unlock_shared();

					++sharedCounter;
				}
			}
			else
			{
				auto timePoint = steady_clock::now();
				timePoint += waitTime;
				if (countMutex.try_lock_shared_until(timePoint))
				{
					countMutex.unlock_shared();

					++sharedCounter;
				}
			}
		}

	std::cout << sharedCounter << ' ' \
		<< exclusiveCounter << std::endl;
#endif

	for (decltype(size) index = 0; \
		index < size; ++index)
	{
		auto& thread = threadPool[index];
		if (thread.joinable())
			thread.join();
	}

	std::cout << ::sharedCounter << ' ' \
		<< ::exclusiveCounter << std::endl;

	delete[] threadPool;
	return EXIT_SUCCESS;
}
