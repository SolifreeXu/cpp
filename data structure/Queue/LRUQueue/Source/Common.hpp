#pragma once

#include "Version.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

#if CXX_VERSION >= CXX_2020
#include <version>

#ifdef __cpp_lib_source_location
#include <source_location>
#include <ostream>
#endif
#endif

// 字符串化
#define STRING(content) #content

// 拼接
#define SPLICE(front, back) front##back

// 弃用
#if CXX_VERSION >= CXX_2014
#define DEPRECATED \
[[deprecated("The name for this item is deprecated.")]]

#else
#define DEPRECATED
#endif

// 替换
#if CXX_VERSION >= CXX_2014
#define REPLACEMENT(signature) \
[[deprecated("The name for this item is deprecated. " \
"Instead, use the name: " STRING(signature) ".")]]

#else
#define REPLACEMENT(signature)
#endif

// 不弃
#if CXX_VERSION >= CXX_2017
#define NODISCARD [[nodiscard]]
#else
#define NODISCARD
#endif

// 自定义名称空间
#define ETERFREE_SPACE Eterfree
#define ETERFREE_SPACE_BEGIN namespace ETERFREE_SPACE {
#define ETERFREE_SPACE_END }
#define USING_ETERFREE_SPACE using namespace ETERFREE_SPACE;

namespace std
{
#if CXX_VERSION < CXX_2017
	template <bool value>
	using bool_constant = integral_constant<bool, value>;
#endif

#if CXX_VERSION < CXX_2017
	//template <typename _Type, const decltype(sizeof 0) _SIZE>
	//NODISCARD constexpr auto size(_Type(&_array)[_SIZE]) noexcept
	//{
	//	return sizeof _array / sizeof _array[0];
	//}

	template <typename _Type, const size_t _SIZE>
	NODISCARD constexpr auto size(_Type(&_array)[_SIZE]) noexcept
	{
		return _SIZE;
	}
#endif

#ifndef __cpp_lib_ssize
	template <typename _Type, const ptrdiff_t _SIZE>
	NODISCARD constexpr auto ssize(_Type(&_array)[_SIZE]) noexcept
	{
		return _SIZE;
	}
#endif

#if CXX_VERSION < CXX_2020
	template <typename _Type, typename... _Args>
	constexpr _Type* construct_at(_Type* _pointer, _Args&&... _args)
	{
		return new(_pointer) _Type(forward<_Args>(_args)...);
	}
#endif

#if CXX_VERSION < CXX_2017
	template <typename _Type>
	constexpr void destroy_at(_Type* _pointer)
	{
		_pointer->~_Type();
	}
#endif
}

ETERFREE_SPACE_BEGIN

/*
* is_arithmetic_v = is_integral_v or is_floating_point_v
* is_member_pointer_v = is_member_object_pointer_v or is_member_function_pointer_v
*/
template <typename _Type>
inline constexpr bool isBase()
{
#if CXX_VERSION >= CXX_2017
	return std::is_arithmetic_v<_Type> \
		|| std::is_enum_v<_Type> \
		|| std::is_pointer_v<_Type> \
		|| std::is_member_pointer_v<_Type>;
#else
	return std::is_arithmetic<_Type>::value \
		|| std::is_enum<_Type>::value \
		|| std::is_pointer<_Type>::value \
		|| std::is_member_pointer<_Type>::value;
#endif
}

template <typename _Type>
struct is_base : std::bool_constant<isBase<_Type>()> {};

#if CXX_VERSION >= CXX_2017
template <typename _Type>
inline constexpr bool is_base_v = is_base<_Type>::value;
#endif

#if CXX_VERSION >= CXX_2020 && defined(__cpp_lib_source_location)
inline std::ostream& operator<<(std::ostream& _stream, \
	const std::source_location& _location)
{
	return _stream << "in " << _location.function_name() \
		<< " at " << _location.file_name() << '(' \
		<< _location.line() << ',' \
		<< _location.column() << "): ";
}
#endif

ETERFREE_SPACE_END
