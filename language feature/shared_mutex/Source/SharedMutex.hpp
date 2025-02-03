#pragma once

#include "Version.hpp"

#if CXX_VERSION < CXX_2014 && !defined(__cpp_lib_shared_timed_mutex)
#define ETERFREE_SHARED_TIMED_MUTEX
#endif

#if defined(ETERFREE_SHARED_TIMED_MUTEX) \
	|| CXX_VERSION < CXX_2017 && !defined(__cpp_lib_shared_mutex)
#define ETERFREE_SHARED_MUTEX
#endif

#if defined(ETERFREE_SHARED_MUTEX) || defined(ETERFREE_SHARED_TIMED_MUTEX)
#include "Common.hpp"
#endif

#ifdef ETERFREE_SHARED_MUTEX
#include <cstddef>
#include <functional>
#include <condition_variable>
#include <mutex>
#endif

#ifdef ETERFREE_SHARED_TIMED_MUTEX
#include <chrono>
#endif

#ifdef ETERFREE_SHARED_MUTEX
// std::mutex在一个线程被锁定，不可以在另一个线程被释放
//class SharedMutex
//{
//	std::mutex _fairMutex;
//	std::mutex _dataMutex;
//	std::mutex _countMutex;
//	std::size_t _counter;

//private:
//	SharedMutex() : \
//		_counter(0) {}
//}

class SharedMutex
{
protected:
	using Predicate = std::function<bool()>;

protected:
	std::mutex _mutex;
	std::condition_variable _singleQueue;
	std::condition_variable _batchQueue;
	std::size_t _counter;
	bool _flag;

	Predicate _exclusive;
	Predicate _unshared;
	Predicate _shareable;

protected:
	NODISCARD bool tryLock() noexcept;

	NODISCARD bool tryLockShared() noexcept;

public:
	SharedMutex();

	SharedMutex(const SharedMutex&) = delete;

	virtual ~SharedMutex() {}

	SharedMutex& operator=(const SharedMutex&) = delete;

	void lock();

	NODISCARD bool try_lock()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return tryLock();
	}

	void unlock();

	void lock_shared();

	NODISCARD bool try_lock_shared()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return tryLockShared();
	}

	void unlock_shared();
};
#endif

#ifdef ETERFREE_SHARED_TIMED_MUTEX
class SharedTimedMutex final : public SharedMutex
{
	template <typename _Rep, typename _Period>
	static NODISCARD auto getDuration(const std::chrono::steady_clock::time_point& _timePoint, \
		const std::chrono::duration<_Rep, _Period>& _duration) -> std::chrono::nanoseconds;

public:
	SharedTimedMutex() = default;

	SharedTimedMutex(const SharedTimedMutex&) = delete;

	SharedTimedMutex& operator=(const SharedTimedMutex&) = delete;

	template <typename _Rep, typename _Period>
	NODISCARD bool try_lock_for(const std::chrono::duration<_Rep, _Period>& _duration);

	template <typename _Clock, typename _Duration>
	NODISCARD bool try_lock_until(const std::chrono::time_point<_Clock, _Duration>& _timePoint);

	template <typename _Rep, typename _Period>
	NODISCARD bool try_lock_shared_for(const std::chrono::duration<_Rep, _Period>& _duration);

	template <typename _Clock, typename _Duration>
	NODISCARD bool try_lock_shared_until(const std::chrono::time_point<_Clock, _Duration>& _timePoint);
};

template <typename _Rep, typename _Period>
auto SharedTimedMutex::getDuration(const std::chrono::steady_clock::time_point& _timePoint, \
	const std::chrono::duration<_Rep, _Period>& _duration) -> std::chrono::nanoseconds
{
	using namespace std::chrono;

	auto duration = steady_clock::now() - _timePoint;

	using Duration = decltype(duration);

	duration = _duration > duration ? _duration - duration : Duration::zero();
	return duration_cast<nanoseconds>(duration);
}

template <typename _Rep, typename _Period>
bool SharedTimedMutex::try_lock_for(const std::chrono::duration<_Rep, _Period>& _duration)
{
	using namespace std::chrono;

	std::unique_lock<std::mutex> lock(_mutex);

	auto timePoint = steady_clock::now();
	auto duration = getDuration(timePoint, _duration);

	using Duration = decltype(duration);

	if (duration == Duration::zero()) return tryLock();

	if (!_batchQueue.wait_for(lock, duration, _exclusive)) return false;

	auto flag = _flag = true;

	timePoint = steady_clock::now();
	duration = getDuration(timePoint, _duration);
	if (duration == Duration::zero())
		if (flag = _unshared()) return true;

	if (!flag || !_singleQueue.wait_for(lock, duration, _unshared))
	{
		_flag = false;
		lock.unlock();

		_batchQueue.notify_all();
		return false;
	}
	return true;
}

template <typename _Clock, typename _Duration>
bool SharedTimedMutex::try_lock_until(const std::chrono::time_point<_Clock, _Duration>& _timePoint)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (!_batchQueue.wait_until(lock, _timePoint, _exclusive)) return false;

	_flag = true;

	if (!_singleQueue.wait_until(lock, _timePoint, _unshared))
	{
		_flag = false;
		lock.unlock();

		_batchQueue.notify_all();
		return false;
	}
	return true;
}

template <typename _Rep, typename _Period>
bool SharedTimedMutex::try_lock_shared_for(const std::chrono::duration<_Rep, _Period>& _duration)
{
	std::unique_lock<std::mutex> lock(_mutex);

	auto timePoint = std::chrono::steady_clock::now();
	auto duration = getDuration(timePoint, _duration);

	using Duration = decltype(duration);

	if (duration == Duration::zero()) return tryLockShared();

	if (!_batchQueue.wait_for(lock, duration, _shareable)) return false;

	++_counter;
	return true;
}

template <typename _Clock, typename _Duration>
bool SharedTimedMutex::try_lock_shared_until(const std::chrono::time_point<_Clock, _Duration>& _timePoint)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (!_batchQueue.wait_until(lock, _timePoint, _shareable)) return false;

	++_counter;
	return true;
}
#endif
