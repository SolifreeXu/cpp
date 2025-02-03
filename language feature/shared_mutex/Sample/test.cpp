#define SHARED_MUTEX
#define SHARED_TIMED_MUTEX

#include "Common.hpp"
#include "System.h"
#include "Version.hpp"

#ifdef SHARED_MUTEX
#include "shared_mutex.hpp"
//#include <shared_mutex>
#endif

#include <cstdlib>
#include <cstddef>
#include <queue>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#ifdef OS_WINDOWS
#include <Windows.h>
#pragma comment(lib, "WinMM.Lib")
#endif

#if CXX_VERSION >= CXX_2017 || defined(__cpp_deduction_guides)
#define DEDUCTION_GUIDE
#endif

#ifdef SHARED_MUTEX
template <typename _MutexType>
using SharedLock = std::shared_lock<_MutexType>;
#else
template <typename _MutexType>
using SharedLock = std::lock_guard<_MutexType>;
#endif

#ifdef SHARED_MUTEX
#ifdef SHARED_TIMED_MUTEX
using MutexType = std::shared_timed_mutex;
#else
using MutexType = std::shared_mutex;
#endif

#else
using MutexType = std::mutex;
#endif

using TaskType = std::size_t(*)();
using TimePoint = std::chrono::steady_clock::time_point;
using Nanosecond = std::chrono::nanoseconds;
using Duration = Nanosecond::rep;

static constexpr auto TOTAL_SIZE = 10000;
static constexpr auto EXCLUSIVE_SIZE = 100;

static constexpr Duration SLEEP_TIME = 1000000;

static MutexType countMutex;
static std::size_t sharedCounter = 0;
static std::size_t exclusiveCounter = 0;

static std::mutex queueMutex;
static std::queue<TaskType> taskQueue;

static TimePoint timePoint;

static void sleepFor(Duration _duration)
{
	if (_duration <= 0) return;

#ifdef OS_WINDOWS
	constexpr UINT PERIOD = 1;

	::timeBeginPeriod(PERIOD);
#endif

	auto duration = Nanosecond(_duration);
	std::this_thread::sleep_for(duration);

#ifdef OS_WINDOWS
	::timeEndPeriod(PERIOD);
#endif
}

static NODISCARD std::size_t read()
{
	SharedLock<MutexType> lock(countMutex);
	sleepFor(SLEEP_TIME);
	++sharedCounter;
	return 0;
}

static NODISCARD std::size_t write()
{
#ifdef DEDUCTION_GUIDE
	std::lock_guard lock(countMutex);
#else
	std::lock_guard<MutexType> lock(countMutex);
#endif

	sleepFor(SLEEP_TIME);
	return ++exclusiveCounter;
}

static NODISCARD TaskType getTask()
{
#ifdef DEDUCTION_GUIDE
	std::lock_guard lock(queueMutex);
#else
	std::lock_guard<std::mutex> lock(queueMutex);
#endif

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
		if (count == EXCLUSIVE_SIZE)
		{
			auto duration = steady_clock::now() - timePoint;
			auto time = duration_cast<nanoseconds>(duration);
			std::cout << time.count() << std::endl;
		}

		task = getTask();
	}
}

static void count()
{
#ifdef SHARED_MUTEX
	using namespace std::chrono;

	std::size_t sharedCounter = 0, exclusiveCounter = 0;

	Nanosecond waitTime(SLEEP_TIME);
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
#ifdef SHARED_TIMED_MUTEX
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
#else
			if (countMutex.try_lock())
			{
				countMutex.unlock();

				++exclusiveCounter;
			}
#endif
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
#ifdef SHARED_TIMED_MUTEX
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
#else
			if (countMutex.try_lock_shared())
			{
				countMutex.unlock_shared();

				++sharedCounter;
			}
#endif
		}

	std::cout << sharedCounter << ' ' \
		<< exclusiveCounter << std::endl;
#endif
}

int main()
{
	constexpr auto SIZE = TOTAL_SIZE / EXCLUSIVE_SIZE;

	for (auto index = 0; index < TOTAL_SIZE; ++index)
		if (index % SIZE == 0)
			taskQueue.push(write);
		else
			taskQueue.push(read);

	auto size = std::thread::hardware_concurrency();
	auto threadPool = new std::thread[size];

	timePoint = std::chrono::steady_clock::now();

	for (decltype(size) index = 0; \
		index < size; ++index)
	{
		auto& thread = threadPool[index];
		thread = std::thread(execute);
	}

	count();

	for (decltype(size) index = 0; \
		index < size; ++index)
	{
		auto& thread = threadPool[index];
		if (thread.joinable())
			thread.join();
	}

	std::cout << sharedCounter << ' ' \
		<< exclusiveCounter << std::endl;

	delete[] threadPool;
	return EXIT_SUCCESS;
}
