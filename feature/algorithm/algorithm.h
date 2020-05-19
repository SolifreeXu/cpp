#pragma once

namespace eterfree
{
	/*
		C++14中的std::move实现例子
		函数返回值类型推导
		标准库的别名模板std:remove_reference_t
	*/
	template <typename T>
	decltype(auto) move(T&& param)
	{
		using ReturnType = std::remove_reference_t<T>&&;
		return static_cast<ReturnType>(param);
	}
	
	//template <typename T>
	//inline void swap(T &x, T &y)
	//{
	//	x ^= y;
	//	y ^= x;
	//	x ^= y;
	//}

	// STL源码形式

	template <typename T>
	inline void swap(T &left, T &right)
	{
		int temp = left;
		left = right;
		right = temp;
	}

	template <typename T>
	inline const T& max(const T& left, const T& right)
	{
		return left < right ? right : left;
	}

	template <typename T>
	inline const T& min(const T& left, const T& right)
	{
		return right < left ? right : left;
	}
}