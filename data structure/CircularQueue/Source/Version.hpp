#pragma once

#include "Compiler.hpp"

#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
// GCC或者Clang

#ifdef __cplusplus
#if __cplusplus >= 201103L
#define HAS_CXX11

#if __cplusplus >= 201402L
#define HAS_CXX14

#if __cplusplus >= 201703L
#define HAS_CXX17

#if __cplusplus >= 202002L
#define HAS_CXX20
#endif
#endif
#endif
#endif
#endif // __cplusplus

#elif defined(COMPILER_MSVC)
// MSVC

#include <vcruntime.h>

#define HAS_CXX14

#if defined(_HAS_CXX17) && _HAS_CXX17
#define HAS_CXX17

#if defined(_HAS_CXX20) && _HAS_CXX20
#define HAS_CXX20

#if defined(_HAS_CXX23) && _HAS_CXX23
#define HAS_CXX23
#endif
#endif
#endif

#else
// 其他或者未知编译器
#endif
