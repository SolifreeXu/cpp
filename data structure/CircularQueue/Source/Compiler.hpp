#pragma once

#if defined(__GNUC__)
// GCC����Clang

#if defined(__clang__) // Clang
#define COMPILER_CLANG
#else // GCC
#define COMPILER_GCC
#endif

#elif defined(_MSC_VER)
// MSVC
#define COMPILER_MSVC

#else
// ��������δ֪������
#endif
