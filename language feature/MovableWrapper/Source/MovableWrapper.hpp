#pragma once

#include "Common.hpp"

#include <type_traits>
#include <utility>

template <typename _ValueType>
class MovableWrapper final
{
	using ValueType = _ValueType;
	using DefaultConstructible = std::is_nothrow_default_constructible<ValueType>;
	using MoveConstructible = std::is_nothrow_move_constructible<ValueType>;
	using Destructible = std::is_nothrow_destructible<ValueType>;
	using MoveAssignable = std::is_nothrow_move_assignable<ValueType>;

private:
	char _data[sizeof(ValueType)];

private:
	static NODISCARD ValueType* convert(const char* _data) noexcept
	{
		auto data = const_cast<char*>(_data);
		return reinterpret_cast<ValueType*>(data);
	}

public:
	MovableWrapper() noexcept(DefaultConstructible::value)
	{
		std::construct_at(get());
	}

	MovableWrapper(ValueType&& _value) noexcept(MoveConstructible::value)
	{
		std::construct_at(get(), std::forward<ValueType>(_value));
	}

	MovableWrapper(const MovableWrapper& _wrapper) noexcept(MoveConstructible::value)
	{
		auto pointer = convert(_wrapper._data);
		std::construct_at(get(), std::move(*pointer));
	}

	MovableWrapper(MovableWrapper&& _wrapper) noexcept(MoveConstructible::value)
	{
		std::construct_at(get(), std::move(*_wrapper));
	}

	~MovableWrapper() noexcept(Destructible::value)
	{
		std::destroy_at(get());
	}

	MovableWrapper& operator=(const MovableWrapper& _wrapper) noexcept(MoveAssignable::value);

	MovableWrapper& operator=(MovableWrapper&& _wrapper) noexcept(MoveAssignable::value)
	{
		*get() = std::move(*_wrapper);
		return *this;
	}

	const ValueType* operator->() const noexcept
	{
		return get();
	}

	ValueType* operator->() noexcept
	{
		return get();
	}

	const ValueType& operator*() const noexcept
	{
		return *get();
	}

	ValueType& operator*() noexcept
	{
		return *get();
	}

	NODISCARD const ValueType* get() const noexcept
	{
		return reinterpret_cast<const ValueType*>(_data);
	}

	NODISCARD ValueType* get() noexcept
	{
		return reinterpret_cast<ValueType*>(_data);
	}

	ValueType release() noexcept(MoveConstructible::value)
	{
		return std::move(*get());
	}

	void reset() noexcept(Destructible::value && DefaultConstructible::value);

	void reset(ValueType&& _value) noexcept(MoveAssignable::value)
	{
		*get() = std::forward<ValueType>(_value);
	}

	void swap(MovableWrapper& _wrapper) noexcept(MoveConstructible::value && MoveAssignable::value);
};

template <typename _ValueType>
auto MovableWrapper<_ValueType>::operator=(const MovableWrapper& _wrapper) noexcept(MoveAssignable::value) \
-> MovableWrapper&
{
	auto pointer = convert(_wrapper._data);
	*get() = std::move(*pointer);
	return *this;
}

template <typename _ValueType>
void MovableWrapper<_ValueType>::reset() noexcept(Destructible::value && DefaultConstructible::value)
{
	auto pointer = get();
	std::destroy_at(pointer);
	std::construct_at(pointer);
}

template <typename _ValueType>
void MovableWrapper<_ValueType>::swap(MovableWrapper& _wrapper) noexcept(MoveConstructible::value && MoveAssignable::value)
{
	auto pointer = get();
	auto value = std::move(*pointer);
	*pointer = std::move(*_wrapper);
	*_wrapper = std::move(value);
}

namespace std
{
	template <typename _Type>
	void swap(MovableWrapper<_Type>& _left, MovableWrapper<_Type>& _right) noexcept(noexcept(_left.swap(_right)))
	{
		_left.swap(_right);
	}
}
