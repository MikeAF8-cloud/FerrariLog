#pragma once

#if (_WIN32)
#include <WinBase.h>

#include "Sink.h"

namespace FerrariLog {
	namespace Sinks {

		template<typename Mutex>
		class MSVCSink : public Sink
		{
		public:
			void SinkLog(Loggers::LogMessage& msg) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);

				OutputDebugString(Sink::m_Formatter->Format(msg).c_str());
			}

			void Flush() override {}

			void SetPattern(const std::string& pattern) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::unique_ptr<Formatters::Formatter>(new Formatters::PatternFormatter(pattern));
			}

			void SetFormatter(std::unique_ptr <Formatters::Formatter>sinkFormatter) override
			{
				std::lock_guard<Mutex> lock(m_Mutex);
				m_Formatter = std::move(sinkFormatter);
			}
		};

		using msvc_st = MSVCSink<OS::NullMutex>;
		using msvc_mt = MSVCSink<std::mutex>;
	}
}
#endif