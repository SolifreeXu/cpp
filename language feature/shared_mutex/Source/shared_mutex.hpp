#pragma once

#include "SharedMutex.h"

#include <chrono>
#include <utility>
#include <exception>
#include <system_error>

namespace std
{
	class shared_mutex
	{
	public:
		using native_handle_type = SharedMutex*;

	private:
		SharedMutex _mutex;

	public:
		shared_mutex() = default;

		shared_mutex(const shared_mutex&) = delete;

		shared_mutex& operator=(const shared_mutex&) = delete;

		void lock()
		{
			_mutex.lock();
		}

		bool try_lock()
		{
			return _mutex.try_lock();
		}

		void unlock()
		{
			_mutex.unlock();
		}

		void lock_shared()
		{
			_mutex.lock_shared();
		}

		bool try_lock_shared()
		{
			return _mutex.try_lock_shared();
		}

		void unlock_shared()
		{
			_mutex.unlock_shared();
		}

		native_handle_type native_handle() noexcept
		{
			return &_mutex;
		}
	};

	class shared_timed_mutex
	{
		SharedTimedMutex _mutex;

	public:
		shared_timed_mutex() = default;

		shared_timed_mutex(const shared_timed_mutex&) = delete;

		shared_timed_mutex& operator=(const shared_timed_mutex&) = delete;

		void lock()
		{
			_mutex.lock();
		}

		bool try_lock()
		{
			return _mutex.try_lock();
		}

		template <typename _Rep, typename _Period>
		bool try_lock_for(const chrono::duration<_Rep, _Period>& _duration)
		{
			return _mutex.try_lock_for(_duration);
		}

		template <typename _Clock, typename _Duration>
		bool try_lock_until(const chrono::time_point<_Clock, _Duration>& _timePoint)
		{
			return _mutex.try_lock_until(_timePoint);
		}

		void unlock()
		{
			_mutex.unlock();
		}

		void lock_shared()
		{
			_mutex.lock_shared();
		}

		bool try_lock_shared()
		{
			return _mutex.try_lock_shared();
		}

		template <typename _Rep, typename _Period>
		bool try_lock_shared_for(const chrono::duration<_Rep, _Period>& _duration)
		{
			return _mutex.try_lock_shared_for(_duration);
		}

		template <typename _Clock, typename _Duration>
		bool try_lock_shared_until(const chrono::time_point<_Clock, _Duration>& _timePoint)
		{
			return _mutex.try_lock_shared_until(_timePoint);
		}

		void unlock_shared()
		{
			_mutex.unlock_shared();
		}
	};

	template <typename _mutex_type>
	class shared_lock
	{
	public:
		using mutex_type = _mutex_type;

	private:
		mutex_type* _mutex;
		bool _locked;

	private:
		static void throw_system_error(errc _errc)
		{
			throw system_error{ make_error_code(_errc) };
		}

	private:
		void validate() const
		{
			if (_mutex == nullptr)
				throw_system_error(errc::operation_not_permitted);

			if (_locked)
				throw_system_error(errc::resource_deadlock_would_occur);
		}

		void try_unlock() noexcept
		{
			if (not _locked \
				or _mutex == nullptr)
				return;

			try
			{
				_mutex->unlock_shared();
				_locked = false;
			}
			catch (exception&) {}
		}

	public:
		shared_lock() noexcept : \
			_mutex(nullptr), _locked(false) {}

		shared_lock(const shared_lock&) = delete;

		shared_lock(shared_lock&& _another) noexcept : \
			_mutex(_another._mutex), _locked(_another._locked)
		{
			_another._mutex = nullptr;
			_another._locked = false;
		}

		explicit shared_lock(mutex_type& _mutex) : \
			_mutex(&_mutex), _locked(true)
		{
			_mutex.lock_shared();
		}

		shared_lock(mutex_type& _mutex, defer_lock_t) noexcept : \
			_mutex(&_mutex), _locked(false) {}

		shared_lock(mutex_type& _mutex, try_to_lock_t) : \
			_mutex(&_mutex), _locked(_mutex.try_lock_shared()) {}

		shared_lock(mutex_type& _mutex, adopt_lock_t) : \
			_mutex(&_mutex), _locked(true) {}

		~shared_lock()
		{
			try_unlock();
		}

		shared_lock& operator=(const shared_lock&) = delete;

		shared_lock& operator=(shared_lock&& _lock) noexcept
		{
			if (&_lock != this)
			{
				try_unlock();

				this->_mutex = _lock._mutex;
				this->_locked = _lock._locked;
				_lock._mutex = nullptr;
				_lock._locked = false;
			}
			return *this;
		}

		explicit operator bool() const noexcept
		{
			return _locked;
		}

		void lock()
		{
			validate();
			_mutex->lock_shared();
			_locked = true;
		}

		bool try_lock()
		{
			validate();
			return _locked = _mutex->try_lock_shared();
		}

		template <typename _Rep, typename _Period>
		bool try_lock_for(const chrono::duration<_Rep, _Period>& _duration)
		{
			validate();
			return _locked = _mutex->try_lock_shared_for(_duration);
		}

		template <typename _Clock, typename _Duration>
		bool try_lock_until(const chrono::time_point<_Clock, _Duration>& _timePoint)
		{
			validate();
			return _locked = _mutex->try_lock_shared_until(_timePoint);
		}

		void unlock()
		{
			if (not _locked or _mutex == nullptr)
				throw_system_error(errc::operation_not_permitted);

			_mutex->unlock_shared();
			_locked = false;
		}

		void swap(shared_lock& _lock) noexcept
		{
			swap(this->_mutex, _lock._mutex);
			swap(this->_locked, _lock._locked);
		}

		mutex_type* release() noexcept
		{
			auto mutex = _mutex;
			_mutex = nullptr;
			_locked = false;
			return mutex;
		}

		mutex_type* mutex() const noexcept
		{
			return _mutex;
		}

		bool owns_lock() const noexcept
		{
			return _locked;
		}
	};

	template <typename _mutex_type>
	void swap(shared_lock<_mutex_type>& left, \
		shared_lock<_mutex_type>& right) noexcept
	{
		left.swap(right);
	}
}
