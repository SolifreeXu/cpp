#pragma once

#include <utility>
#include <cstddef>
#include <system_error>
#include <mutex>
#include <condition_variable>

#if _HAS_NODISCARD
#define _NODISCARD [[nodiscard]]
#else // ^^^ CAN HAZ [[nodiscard]] / NO CAN HAZ [[nodiscard]] vvv
#define _NODISCARD
#endif // _HAS_NODISCARD

#define ETERFREE_BEGIN namespace eterfree {
#define ETERFREE_END }

ETERFREE_BEGIN

class shared_mutex
{
	std::mutex mutex;
	std::condition_variable shared_variable;
	std::condition_variable exclusive_variable;
	size_t shared_counter;
	size_t exclusive_counter;
	bool exclusive;
private:
	bool shareable() const noexcept
	{
		return !exclusive;
	}

	bool excludable() const noexcept
	{
		return !exclusive && shared_counter == 0;
	}
public:
	shared_mutex() noexcept
		: shared_counter(0), exclusive_counter(0), exclusive(false) {}
	shared_mutex(const shared_mutex&) = delete;
	shared_mutex& operator=(const shared_mutex&) = delete;

	void lock() noexcept
	{
		std::unique_lock<std::mutex> lock(mutex);
		++exclusive_counter;
		exclusive_variable.wait(lock, [this]() { return excludable(); });
		exclusive = true;
	}

	_NODISCARD bool try_lock() noexcept
	{
		std::unique_lock<std::mutex> lock(mutex);
		if (!excludable())
			return false;

		++exclusive_counter;
		return exclusive = true;
	}

	void unlock() noexcept
	{
		std::unique_lock<std::mutex> lock(mutex);
		exclusive = false;
		bool shared = --exclusive_counter == 0;
		lock.unlock();

		if (shared)
			shared_variable.notify_all();
		else
			exclusive_variable.notify_one();
	}

	void lock_shared() noexcept
	{
		std::unique_lock<std::mutex> lock(mutex);
		shared_variable.wait(lock, [this]() { return shareable(); });
		++shared_counter;
	}

	_NODISCARD bool try_lock_shared() noexcept
	{
		std::unique_lock<std::mutex> lock(mutex);
		if (!shareable())
			return false;

		++shared_counter;
		return true;
	}

	void unlock_shared() noexcept
	{
		std::unique_lock<std::mutex> lock(mutex);
		bool exclusive = --shared_counter == 0 && exclusive_counter > 0;
		lock.unlock();

		if (exclusive)
			exclusive_variable.notify_one();
	}
};

template <typename mutex_type>
class shared_lock
{
	mutex_type* handle;
	bool locked;
private:
	static inline void throw_system_error(const std::errc errc)
	{
		throw std::system_error{ std::make_error_code(errc) };
	}

	void validate() const
	{
		if (handle == nullptr)
			throw_system_error(std::errc::operation_not_permitted);

		if (locked)
			throw_system_error(std::errc::resource_deadlock_would_occur);
	}
public:
	shared_lock() noexcept : handle(nullptr), locked(false) {}

	shared_lock(const shared_lock&) = delete;

	shared_lock(shared_lock&& another) noexcept
		: handle(another.handle), locked(another.locked)
	{
		another.handle = nullptr;
		another.locked = false;
	}

	explicit shared_lock(mutex_type& mutex)
		: handle(&mutex), locked(false)
	{
		mutex.lock_shared();
		locked = true;
	}

	shared_lock(mutex_type& mutex, std::adopt_lock_t)
		: handle(&mutex), locked(true) {}

	shared_lock(mutex_type& mutex, std::defer_lock_t) noexcept
		: handle(&mutex), locked(false) {}

	shared_lock(mutex_type& mutex, std::try_to_lock_t)
		: handle(&mutex), locked(mutex.try_lock_shared()) {}

	~shared_lock() noexcept
	{
		if (locked)
			handle->unlock_shared();
	}

	shared_lock& operator=(const shared_lock&) = delete;

	shared_lock& operator=(shared_lock&& another) noexcept
	{
		if (this == &another)
			return *this;

		this->~shared_lock();
		handle = another.handle;
		locked = another.locked;
		another.handle = nullptr;
		another.locked = false;
		return *this;
	}

	void lock()
	{
		validate();
		handle->lock_shared();
		locked = true;
	}

	_NODISCARD bool try_lock()
	{
		validate();
		return locked = handle->try_lock_shared();
	}

	void unlock()
	{
		if (handle == nullptr || !locked)
			throw_system_error(std::errc::operation_not_permitted);

		handle->unlock_shared();
		locked = false;
	}

	void swap(shared_lock& another) noexcept
	{
		std::swap(handle, another.handle);
		std::swap(locked, another.locked);
	}

	mutex_type* release() noexcept
	{
		mutex_type* handle = this->handle;
		this->handle = nullptr;
		locked = false;
		return handle;
	}

	_NODISCARD bool owns_lock() const noexcept
	{
		return locked;
	}

	explicit operator bool() const noexcept
	{
		return locked;
	}

	_NODISCARD mutex_type* mutex() const noexcept
	{
		return handle;
	}
};

template <typename mutex_type>
void swap(shared_lock<mutex_type>& left, shared_lock<mutex_type>& right) noexcept
{
	left.swap(right);
}

ETERFREE_END
