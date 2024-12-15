#pragma once

#include "Compiler.hpp"

#define CXX_1998 199711L
#define CXX_2011 201103L
#define CXX_2014 201402L
#define CXX_2017 201703L
#define CXX_2020 202002L

#if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
// GCC或者Clang

#if __cplusplus >= CXX_2020
#define CXX_VERSION CXX_2020
#elif __cplusplus >= CXX_2017
#define CXX_VERSION CXX_2017
#elif __cplusplus >= CXX_2014
#define CXX_VERSION CXX_2014
#elif __cplusplus >= CXX_2011
#define CXX_VERSION CXX_2011
#elif __cplusplus >= CXX_1998
#define CXX_VERSION CXX_1998
#endif

#elif defined(COMPILER_MSVC)
// MSVC

#define MSVC_2005 1400
#define MSVC_2008 1500
#define MSVC_2010 1600 
#define MSVC_2012 1700
#define MSVC_2013 1800
#define MSVC_2015 1900
#define MSVC_2017 1910
#define MSVC_2019 1920
#define MSVC_2022 1930

#if defined(_MSVC_LANG)
#if _MSVC_LANG >= CXX_2020
#define CXX_VERSION CXX_2020
#elif _MSVC_LANG >= CXX_2017
#define CXX_VERSION CXX_2017
#elif _MSVC_LANG >= CXX_2014
#define CXX_VERSION CXX_2014
#elif _MSVC_LANG >= CXX_2011
#define CXX_VERSION CXX_2011
#elif _MSVC_LANG >= CXX_1998
#define CXX_VERSION CXX_1998
#endif

#elif defined(_MSC_VER)
#if _MSC_VER >= MSVC_2013
#define CXX_VERSION CXX_2011
#else
#define CXX_VERSION CXX_1998
#endif
#endif

#else
// 其他或者未知编译器
#endif
