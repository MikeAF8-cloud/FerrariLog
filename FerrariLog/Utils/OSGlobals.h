#pragma once

#include <cstdio>
#include <mutex>

#include "Macros.h"

namespace FerrariLog {
	namespace OS {

		struct ConsoleOutput
		{
#ifdef _WIN32
			static HANDLE Handle()
			{
				return ::GetStdHandle(STD_OUTPUT_HANDLE);
			}
#endif
			static std::FILE* Stream()
			{
				return stdout;
			}
		};

		struct ConsoleError
		{
#ifdef _WIN32
			static HANDLE Handle()
			{
				return ::GetStdHandle(STD_ERROR_HANDLE);
			}
#endif
			static std::FILE* Stream()
			{
				return stderr;
			}
		};

		struct ConsoleInput
		{
#ifdef _WIN32
			static HANDLE Handle()
			{
				return ::GetStdHandle(STD_INPUT_HANDLE);
			}
#endif
			static std::FILE* Stream()
			{
				return stdin;
			}
		};

		struct ConsoleMutex
		{
			using Mutex = std::mutex;
			static Mutex& GetMutex()
			{
				static Mutex m_Mutex;
				return m_Mutex;
			}
		};

		struct NullMutex
		{
			void lock() {}
			void unlock() {}
			bool try_lock()
			{
				return true;
			}
		};

		struct ConsoleNullMutex
		{
			using Mutex = NullMutex;
			static Mutex& GetMutex()
			{
				static Mutex m_Mutex;
				return m_Mutex;
			}
		};
	}
}