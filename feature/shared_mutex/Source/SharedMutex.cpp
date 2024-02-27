#include "SharedMutex.h"

#include <cstdint>

bool SharedMutex::tryLock() noexcept
{
	if (_flag or _counter > 0)
		return false;
	return _flag = true;
}

bool SharedMutex::tryLockShared() noexcept
{
	if (_flag or _counter == SIZE_MAX)
		return false;

	++_counter;
	return true;
}

SharedMutex::SharedMutex() : \
	_counter(0), _flag(false)
{
	_exclusive = [this]
	{
		return not _flag;
	};

	_unshared = [this]
	{
		return _counter == 0;
	};

	_shareable = [this]
	{
		return not _flag \
			and _counter < SIZE_MAX;
	};
}

void SharedMutex::lock()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_batchQueue.wait(lock, _exclusive);

	_flag = true;

	_singleQueue.wait(lock, _unshared);
}

void SharedMutex::unlock()
{
	_mutex.lock();
	_flag = false;
	_mutex.unlock();

	_batchQueue.notify_all();
}

void SharedMutex::lock_shared()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_batchQueue.wait(lock, _shareable);

	++_counter;
}

void SharedMutex::unlock_shared()
{
	_mutex.lock();
	auto count = --_counter;
	auto exclusive = _flag;
	_mutex.unlock();

	if (exclusive)
	{
		if (count == 0)
			_singleQueue.notify_one();
	}
	else if (not exclusive)
	{
		constexpr auto MAX_SIZE = SIZE_MAX - 1;
		if (count == MAX_SIZE)
			_batchQueue.notify_all();
	}
}
