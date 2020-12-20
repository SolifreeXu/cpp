#pragma once

#include <type_traits>

#define ETERFREE_BEGIN namespace eterfree {
#define ETERFREE_END }

ETERFREE_BEGIN

/*
 *	��������ֵ�����Ƶ�
 *	��׼�����ģ��std::remove_reference_t
 */
template <typename Type>
inline constexpr auto move(Type&& param) noexcept
{
	return static_cast<std::remove_reference_t<Type>&&>(param);
}

//template <typename Type>
//inline constexpr void swap(Type& left, Type& right) noexcept
//{
//	left ^= right;
//	right ^= left;
//	left ^= right;
//}

// ��׼��Դ����ʽ
template <typename Type>
inline constexpr void swap(Type& left, Type& right) noexcept
{
	Type temp = left;
	left = right;
	right = temp;
}

ETERFREE_END
