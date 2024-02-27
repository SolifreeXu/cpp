#pragma once

#include <chrono>
#include <cstddef>
#include <functional>
#include <condition_variable>
#include <mutex>

// std::mutex在一个线程被锁定，不可以在另一个线程被释放
//class SharedMutex
//{
//	std::mutex _fairMutex;
//	std::mutex _dataMutex;
//	std::mutex _countMutex;
//	std::size_t _counter;

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
	bool tryLock() noexcept;

	bool tryLockShared() noexcept;

public:
	SharedMutex();

	SharedMutex(const SharedMutex&) = delete;

	virtual ~SharedMutex() {}

	SharedMutex& operator=(const SharedMutex&) = delete;

	void lock();

	bool try_lock()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return tryLock();
	}

	void unlock();

	void lock_shared();

	bool try_lock_shared()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return tryLockShared();
	}

	void unlock_shared();
};

class SharedTimedMutex final : public SharedMutex
{
private:
	template <typename _Rep, typename _Period>
	static std::chrono::nanoseconds getDuration(const std::chrono::steady_clock::time_point& _timePoint, \
		const std::chrono::duration<_Rep, _Period>& _duration);

public:
	SharedTimedMutex() = default;

	SharedTimedMutex(const SharedTimedMutex&) = delete;

	SharedTimedMutex& operator=(const SharedTimedMutex&) = delete;

	template <typename _Rep, typename _Period>
	bool try_lock_for(const std::chrono::duration<_Rep, _Period>& _duration);

	template <typename _Clock, typename _Duration>
	bool try_lock_until(const std::chrono::time_point<_Clock, _Duration>& _timePoint);

	template <typename _Rep, typename _Period>
	bool try_lock_shared_for(const std::chrono::duration<_Rep, _Period>& _duration);

	template <typename _Clock, typename _Duration>
	bool try_lock_shared_until(const std::chrono::time_point<_Clock, _Duration>& _timePoint);
};

template <typename _Rep, typename _Period>
std::chrono::nanoseconds SharedTimedMutex::getDuration(const std::chrono::steady_clock::time_point& _timePoint, \
	const std::chrono::duration<_Rep, _Period>& _duration)
{
	auto duration = std::chrono::steady_clock::now() - _timePoint;
	duration = _duration > duration ? _duration - duration : duration.zero();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
}

template <typename _Rep, typename _Period>
bool SharedTimedMutex::try_lock_for(const std::chrono::duration<_Rep, _Period>& _duration)
{
	auto timePoint = std::chrono::steady_clock::now();

	std::unique_lock<std::mutex> lock(_mutex);

	auto duration = getDuration(timePoint, _duration);
	if (duration == duration.zero()) return tryLock();

	if (not _batchQueue.wait_for(lock, duration, _exclusive)) return false;

	_flag = true;

	auto flag = true;
	duration = getDuration(timePoint, _duration);
	if (duration == duration.zero())
		if (flag = _unshared()) return true;

	if (not flag or not _singleQueue.wait_for(lock, duration, _unshared))
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
	if (not _batchQueue.wait_until(lock, _timePoint, _exclusive)) return false;

	_flag = true;

	if (not _singleQueue.wait_until(lock, _timePoint, _unshared))
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
	auto timePoint = std::chrono::steady_clock::now();

	std::unique_lock<std::mutex> lock(_mutex);

	auto duration = getDuration(timePoint, _duration);
	if (duration == duration.zero()) return tryLockShared();

	if (not _batchQueue.wait_for(lock, duration, _shareable))
		return false;

	++_counter;
	return true;
}

template <typename _Clock, typename _Duration>
bool SharedTimedMutex::try_lock_shared_until(const std::chrono::time_point<_Clock, _Duration>& _timePoint)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (not _batchQueue.wait_until(lock, _timePoint, _shareable)) return false;

	++_counter;
	return true;
}
