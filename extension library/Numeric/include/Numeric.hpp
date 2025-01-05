#pragma once

// 整数类型
enum class IntegerType
{
	ALL,      // 所有
	NATURAL,  // 自然数
	POSITIVE, // 正数
	NEGATIVE, // 负数
};

// 检查合法性
bool valid(const char* _string, IntegerType _type);
