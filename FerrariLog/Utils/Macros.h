#pragma once

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX // prevent windows redefining min/max
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#endif

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) 
#define WINDOWS_OS
#elif defined(__unix__) || defined(__unix) || defined(__linux__) 
#define LINUX_OS
#elif defined(__APPLE__) || defined(__MACH__)
#define APPLE_OS
 #else
 #error Unknown Platform 
 #endif


#ifdef WINDOWS_OS
#include <io.h>
#else
#endif

#ifdef WINDOWS_OS
#include <fileapi.h>
#else
#endif

#if 0 
#define FERRARI_STRRCHR(str, sep) __builtin_strrchr(str, sep)
#else
#define FERRARI_STRRCHR(str, sep) strrchr(str, sep)
#endif

#define FERRARI_LOG_THREADED(x)

#ifdef _WIN32
#define FERRARI_FILENAME(file) FERRARI_STRRCHR("\\" file, '\\') + 1
#else
#define FERRARI_FILENAME(file) FERRARI_STRRCHR("/" file, '/') + 1
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define FERRARI_NOEXCEPTION throw()
#define FERRARI_CONSTEXPRESSION inline
#else
#define FERRARI_NOEXCEPTION noexcept
#define FERRARI_CONSTEXPRESSION constexpr
#endif

#if !defined(FERRARI_EOL)
#ifdef _WIN32
#define FERRARI_EOL "\r\n"
#else
#define FERRARI_EOL "\n"
#endif
#endif

