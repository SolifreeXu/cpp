#pragma once

#if defined(_WIN32)
#define OS_WINDOWS

#else
#if defined(__unix) || defined(__unix__)
#define OS_UNIX
#endif

#if defined(__linux) || defined(__linux__)
#define OS_LINUX
#endif
#endif

#if defined(__ANDROID__)
#define OS_ANDROID
#elif defined(__APPLE__)
#define OS_APPLE
#endif
