#pragma once

#if defined(__GNUC__)
// GCC或者Clang

#if defined(__clang__) // Clang
#define COMPILER_CLANG
#else // GCC
#define COMPILER_GCC
#endif

#elif defined(_MSC_VER)
// MSVC
#define COMPILER_MSVC

#else
// 其他或者未知编译器
#endif
