#pragma once

#include <ctime>
#include <chrono>
#include <memory>
#include <thread>

namespace FerrariLog {
	namespace OS {

		class OS {
		public:
#ifndef FERRARI_GETTIME
	#ifdef FERRARI_LOCALTIME
		#define FERRARI_GETTIME OS::OS::LocalTime()
	#elifdef FERRARI_GMTTIME
		#define FERRARI_GETTIME OS::OS::GMTime();
	#else
		#define FERRARI_GETTIME OS::OS::LocalTime()
	#endif
#endif
			inline static std::tm LocalTime(const std::time_t& localTime)
			{
				std::tm time;
#ifdef _WIN32
				localtime_s(&time, &localTime);
#else
				localtime_r(&localTime, &tm);
#endif
				return time;
			}

			inline static std::tm LocalTime()
			{
				std::time_t currentTime = time(nullptr);
				return LocalTime(currentTime);
			}

			inline static std::chrono::system_clock::time_point CurrentTime() throw()
			{
				return std::chrono::system_clock::now();
			}

			template<typename T>
			inline static unsigned int GetEpochSecond()
			{
				T s = std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch());
				double duration = std::chrono::duration<double>(s).count();
				return (int)duration;
			}
			
			inline static std::tm GMTime(const std::time_t &time) throw()
			{
				std::tm tp;
#ifdef _WIN32
				gmtime_s(&tp, &time);
#else
				gmtime_r(&time, &tm);
#endif
				return tp;
			}

			inline static std::tm GMTime() throw()
			{
				static std::time_t currentTime = time(nullptr);
				return GMTime(currentTime);
			}

			inline size_t GetThreadID() throw()
			{
#ifdef _WIN32
				return static_cast<size_t>(::GetCurrentThreadId());
#elif __linux__
#if defined(__ANDROID__) && defined(__ANDROID_API__) && (__ANDROID_API__ < 21)
#define SYS_gettid __NR_gettid
#endif
				return static_cast<size_t>(syscall(SYS_gettid));
#elif __APPLE__
				uint64_t tid;
				pthread_threadid_np(nullptr, &tid);
				return static_cast<size_t>(tid);
#else // Default to standard C++11 (other Unix)
				return static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
			}
		};
	}
}


