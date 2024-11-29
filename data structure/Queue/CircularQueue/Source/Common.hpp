#pragma once

#include "Version.hpp"

#include <type_traits>
#include <utility>

#define NODISCARD [[nodiscard]]

#ifdef HAS_CXX20
#define CONSTEXPR constexpr
#else // HAS_CXX20
#define CONSTEXPR
#endif // HAS_CXX20

/*
* is_arithmetic_v = is_integral_v || is_floating_point_v
* is_member_pointer_v = is_member_object_pointer_v || is_member_function_pointer_v
*/
template <typename _Type>
constexpr bool is_base_v = std::is_arithmetic_v<_Type> \
or std::is_enum_v<_Type> \
or std::is_pointer_v<_Type> \
or std::is_member_pointer_v<_Type>;

template <typename _Type>
struct is_base : std::bool_constant<is_base_v<_Type>> {};

inline constexpr auto SUBSCRIPT_OUT_OF_RANGE = "subscript out of range";
inline constexpr auto INCREMENT_OUT_OF_RANGE = "can't increment iterator past end";
inline constexpr auto DECREMENT_OUT_OF_RANGE = "can't decrement iterator before begin";
inline constexpr auto DEREFERENCE_OUT_OF_RANGE = "can't dereference out of range iterator";
inline constexpr auto INSERT_OUTSIDE_RANGE = "insert iterator outside range";
inline constexpr auto EMPLACE_OUTSIDE_RANGE = "emplace iterator outside range";
inline constexpr auto ERASE_OUTSIDE_RANGE = "erase iterator outside range";
inline constexpr auto TRANSPOSED_RANGE = "iterator range transposed";
inline constexpr auto SEEK_AFTER_END = "cannot seek iterator after end";
inline constexpr auto SEEK_BEFORE_BEGIN = "cannot seek iterator before begin";
inline constexpr auto DIFFERENT_CONTAINER = "iterators are from different containers";
inline constexpr auto RESERVE_EXCEED_MAXIMUM_SIZE = "reserved capacity exceeds maximum size";
inline constexpr auto FRONT_ON_EMPTY_CONTAINER = "front called on empty container";
inline constexpr auto BACK_ON_EMPTY_CONTAINER = "back called on empty container";
inline constexpr auto POP_FRONT_ON_EMPTY_CONTAINER = "pop_front called on empty container";
inline constexpr auto POP_BACK_ON_EMPTY_CONTAINER = "pop_back called on empty container";

#ifndef HAS_CXX20
namespace std
{
	template <class _Type, class... _Args>
	constexpr _Type* construct_at(_Type* _pointer, _Args&&... _args)
	{
		return new(_pointer) _Type(forward<_Args>(_args)...);
	}
}
#endif // !HAS_CXX20
